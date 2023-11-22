/*******************************************************************************
This program demonstrates the use of mutexes and multiple reader single writer
locks (rwlocks) to coordinate access of shared data by multiple threads spread
among multiple processes.  No data is actually read or written by this program;
it is more a demonstration of timing issues of rw locks vs mutexes, plus an
example of how to use rw locks.

/******************************************************************************/
#include <sys/types.h>		/* For boolean_t type. */
#include <stdio.h>		/* Standard IO Library. */
#include <pthread.h>		/* Posix threads. */
#include <sys/times.h>		/* For struct tms. */
#include <unistd.h>		/* For _SC_CLK_TCK. */
#include <fcntl.h>		/* For file opening definitions. */
#include <sys/mman.h>		/* For mmap definitions. */
#include <errno.h>		/* Error definitions. */
#include <stdlib.h>
#include <string.h>

#include "utils.h"		/* For fractSleep(). */

/* Mmapped file containing the shared data. */
#define SHARED_DATA_FILE "/tmp/proc_rw.dat"

#define NUM_READERS 10		/* Number of reader threads. */
#define NUM_WRITERS 1		/* Number of writer threads. */
#define NUM_THREADS (NUM_READERS + NUM_WRITERS)
#define WRITER NUM_READERS	/* Make the writer the last thread. */

#define LOOP_MAX 10		/* Number of operations per thread. */
#define TIME_FOR_READ 1.0	/* Time delay representing a read. */
#define TIME_FOR_WRITE 0.5	/* Time delay representing a write. */

typedef int boolean_t;
#define B_FALSE 0
#define B_TRUE 1

/* Global stuff. */

/* Definition of global data. */
typedef struct globalStuff {
  pthread_rwlock_t sharedRWLock;	/* Multiple Reader Single Writer lock.*/
  pthread_mutex_t sharedMutex;		/* Mutex for comparison. */
  boolean_t userwlock;			/* Which lock to use. */
  int count;
} globalStuff;

/* Handle to global data. */
struct globalStuff * globalArea;

/* Define threads. */
pthread_t threads[NUM_THREADS];

/* Forward references. */
void doWrite();			/* Write operation. */
void doRead();			/* Read operation. */
void * writerMain(void *);	/* Main for writer thread. */
void * readerMain(void *);	/* Main for reader thread. */


/******************************************************************************/
void doWrite(int id) {
/*******************************************************************************
This function represents a write operation.  It obviously does not actually
write anything, but represents a delay that it might take to perform such an
operation.  For testing locks, it is the delay that matters.
/******************************************************************************/

  char string[23];

  sprintf (string,"Thread %d write start\n", id);
  printWithTime (string);

  fractSleep(TIME_FOR_WRITE);

  sprintf (string,"Thread %d write done\n", id);
  printWithTime (string);
}


/******************************************************************************/
void doRead(int id) {
/*******************************************************************************
This function represents a read operation, modeled as a delay.
/******************************************************************************/

  char string[23];

  sprintf (string,"Thread %d read start\n", id);
  printWithTime (string);

  fractSleep(TIME_FOR_READ);

  sprintf (string,"Thread %d read done\n", id);
  printWithTime (string);
}


/******************************************************************************/
int main(int argc, char *argv[]) {
/*******************************************************************************
Main program.  Create or open existing shared region, possibly initialize
shared locks, and spawn threads.  Time the threads.
/******************************************************************************/

  struct tms junk;	/* Ignored, but needed for calling times(2). */
  clock_t startingTime;	/* Starting time, in ticks. */
  clock_t endingTime;	/* Ending time, in ticks. */

  boolean_t error = B_FALSE;	/* Error on commandline input. */

  /* File handle to mmapped file. */
  int shared_fd;

  /* Attributes for initializing locks as interprocess. */
  pthread_mutexattr_t mutex_attributes;
  pthread_rwlockattr_t rwlock_attributes;

  /* True if this process initializes shared area. */
  boolean_t createdHere = B_FALSE;

  int count;

  /* Open the shared area.  Err out if it already exists (which means we are
  not the first process to use it, and thus not to be the creator if it. */
  shared_fd = open(SHARED_DATA_FILE, (O_RDWR | O_CREAT | O_EXCL), 0777);

  /* Flag that we are the creator if we are, or just open the file otherwise. */
  if ((shared_fd == -1) && (errno == EEXIST)) {
    shared_fd = open(SHARED_DATA_FILE, O_RDWR, 0777);
  } else {
    createdHere = B_TRUE;
  }

  /* Some other error opening the file. */
  if (shared_fd < 0) {
    perror ("Error opening shared data area");
    exit (errno);
  }

  /* Expand the file to hold the shared data, before mmapping it. */
  if (ftruncate(shared_fd,sizeof(struct globalStuff))) {
    perror ("ftruncate");
    exit (errno);
  }

  /* Mmap the file so all processes needing access to it can see it. */
  globalArea = (struct globalStuff *)mmap(
      NULL, sizeof(struct globalStuff), PROT_READ | PROT_WRITE, MAP_SHARED,
      shared_fd, 0);
  if (globalArea == (struct globalStuff *)-1) {
    perror ("Error getting virtual addr for sharedMutex");
  exit (errno);
  }

  /* Check commandline arguments. */
  if (!createdHere) {
    fprintf (stderr, "Using %s locks created by other process.\n",
	(globalArea->userwlock ? "rw" : "mutex"));
  } else {
    if (argc < 2) {
      error = B_TRUE;
    } else if (!(strcmp(argv[1],"rw"))) {
       globalArea->userwlock = B_TRUE;
    } else if (!(strcmp(argv[1],"mutex"))) {
       globalArea->userwlock = B_FALSE;
    } else {
      error = B_TRUE;
    }
  }

  /* Print usage statement and exit on error. */
  if (error) {
    fprintf (stderr, "Usage: \"%s rw\" or \"%s mutex\"\n", argv[0], argv[0]);
    exit (0);
  }

  /* Initialize locks in shared region if we are the shared region creator. */
  if (createdHere) {

    /* Use rwlocks. */
    if (globalArea->userwlock) {

      /* Initialize rwlock attributes list. */
      if (pthread_rwlockattr_init(&rwlock_attributes)) {
        perror ("pthread_rwlockattr_init");
        exit (errno);
      }

      /* Reflect in the attributes that this is an interprocess shared rwlock.*/
      if (pthread_rwlockattr_setpshared(
	  &rwlock_attributes, PTHREAD_PROCESS_SHARED)) {
        perror ("pthread_rwlockattr_setpshared");
        exit (errno);
      }

      /* Initialize the rwlock in the mmapped area, with the attributes above.*/
      if (pthread_rwlock_init(&globalArea->sharedRWLock, &rwlock_attributes)) {
        perror ("pthread_rwlock_init");
        exit (errno);
      }

    /* Use mutexes. */
    } else {

      /* Initialize mutex attributes list. */
      if (pthread_mutexattr_init(&mutex_attributes)) {
        perror ("pthread_mutexattr_init");
        exit (errno);
      }

      /* Reflect in the attributes that this is an interprocess shared rwlock.*/
      if (pthread_mutexattr_setpshared(
	  &mutex_attributes, PTHREAD_PROCESS_SHARED)) {
        perror ("pthread_mutexattr_setpshared");
        exit (errno);
      }

      /* Initialize the mutex in the mmapped area, with the attributes above. */
      if (pthread_mutex_init(&globalArea->sharedMutex, &mutex_attributes)) {
        perror ("pthread_mutex_init");
        exit (errno);
      }
    }
  }

  /* Set up to receive ^C to do proper cleanup handling here (not done). */

  /* Get reference starting time. */
  startingTime = times(&junk);

  /* Create threads.  One writer, many readers. */

  if (pthread_create( &threads[WRITER], NULL, writerMain, (void *)NULL)) {
    perror ("Error starting writer thread");
    exit (errno);
  }

  for (count = 0; count < NUM_READERS; count++) {
    if (pthread_create( &threads[count], NULL, readerMain, (void *)count)) {
      perror ("Error starting reader threads");
      exit (errno);
    }
  }

  /* Wait for threads to finish. */
  for (count = 0; count < NUM_THREADS; count++) {
    pthread_join(threads[count], (void **)NULL);
  }

  /* Get ending time, and calculate and print delta time. */
  endingTime = times(&junk);
  printf ("Execution took %.1f seconds\n",
      ((endingTime - startingTime)/(double)(sysconf(_SC_CLK_TCK))));
}


/******************************************************************************/
void * writerMain(void *idArg) {
/*******************************************************************************
This is the main for the writer thread.  It gets the appropriate lock, does an
operation for a while, then releases the lock.
/******************************************************************************/

  int id = (int)idArg;	/* Arg passed is the thread's ID number. */

  int loopCount;

  for (loopCount = 0; loopCount < LOOP_MAX; loopCount++) {

    /* Get either the rwlock or the mutex, depending on the mode. */
    if (globalArea->userwlock) {
      pthread_rwlock_wrlock(&globalArea->sharedRWLock);
    } else {
      pthread_mutex_lock(&globalArea->sharedMutex);
    }

    /* Hold the lock for a while, simulating use of a shared data structure. */
    doWrite(id);

    /* Release the lock. */
    if (globalArea->userwlock) {
      pthread_rwlock_unlock(&globalArea->sharedRWLock);
    } else {
      pthread_mutex_unlock(&globalArea->sharedMutex);
    }

    /* Delay for a while before iterating again, to stagger the tasks. */
    fractSleep(0.5);
  }
}


/******************************************************************************/
void * readerMain(void * idArg) {
/*******************************************************************************
This is the main for the reader threads.
/******************************************************************************/

  int id = (int)idArg;	/* Arg passed is the thread's ID number. */

  int loopCount;

  for (loopCount = 0; loopCount < LOOP_MAX; loopCount++) {

    /* Get either the rwlock or the mutex, depending on the mode. */
    if (globalArea->userwlock) {
      pthread_rwlock_rdlock(&globalArea->sharedRWLock);
    } else {
      pthread_mutex_lock(&globalArea->sharedMutex);
    }

    doRead(id);

    /* Release the lock. */
    if (globalArea->userwlock) {
      pthread_rwlock_unlock(&globalArea->sharedRWLock);
    } else {
      pthread_mutex_unlock(&globalArea->sharedMutex);
    }

    /* Delay for a while before iterating again, to stagger the tasks. */
    fractSleep(0.5);
  }
}
