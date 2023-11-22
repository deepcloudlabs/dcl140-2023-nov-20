/*******************************************************************************
This program demonstrates the use of multiple reader single writer locks, and
the advantage of using them in situations where reading a shared resource is
done much more often than writing.  Times of reads and writes are simulated by
delays, which cause contention among the readers and writers.

This program can be run using either multiple reader single writer locks, or
with mutexes.  The former case shows a four-fold speedup in execution of all
tasks for this example.
/******************************************************************************/

#include <stdio.h>		/* Standard IO Library. */
#include <pthread.h>		/* Posix threads. */
#include <sys/times.h>		/* For struct tms. */
#include <unistd.h>		/* For _SC_CLK_TCK. */
#include <errno.h>		/* Error definitions. */
#include <stdlib.h>
#include <string.h>

#include "utils.h"		/* for fractSleep() and printWithTime(). */

#define NUM_READERS 10		/* Number of reader threads. */
#define NUM_WRITERS 1		/* Number of writer threads. */
#define NUM_THREADS (NUM_READERS + NUM_WRITERS)
#define WRITER NUM_READERS	/* Make the writer the last thread. */

#define LOOP_MAX 50		/* Number of operations per thread. */
#define TIME_FOR_READ 0.01	/* Time delay representing a read. */
#define TIME_FOR_WRITE 0.01	/* Time delay representing a write. */

/* Global stuff. */

typedef int boolean_t;
#define B_FALSE 0
#define B_TRUE 1

boolean_t userwlock = B_TRUE;	/* Which lock to use. */

/* Multiple Reader Single Writer lock. */
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

/* Mutex for comparison. */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Forward references. */
void doWrite();			/* Write operation. */
void doRead();			/* Read operation. */
void * writerMain(void *);	/* Main for writer thread. */
void * readerMain(void *);	/* Main for reader thread. */

/*******************************************************************************
This function represents a write operation.  It obviously does not
actually write anything, but represents a delay that it might take to
perform such an operation.  For testing locks, it is the delay that
matters.
/******************************************************************************/
void doWrite() {
  fractSleep(TIME_FOR_WRITE);
}


/*******************************************************************************
This function represents a read operation, modeled as a delay.
/******************************************************************************/
void doRead() {
  fractSleep(TIME_FOR_READ);
}


/*******************************************************************************
Main program.  Get commandline arguments and spawn threads.  Time how long it
takes for all threads to complete, and report this back to the user.
/******************************************************************************/
int main(int argc, char *argv[]) {

  struct tms junk;	/* Ignored, but needed for calling times(2). */
  clock_t startingTime;	/* Starting time, in ticks. */
  clock_t endingTime;	/* Ending time, in ticks. */

  boolean_t error = B_FALSE;	/* Error on commandline input. */

  /* Define threads. */
  pthread_t threads[NUM_THREADS];

  int count;

  /* Check commandline arguments. */
  if (argc < 2) {
    error = B_TRUE;
  } else if (!(strcmp(argv[1],"rw"))) {
     userwlock = B_TRUE;
  } else if (!(strcmp(argv[1],"mutex"))) {
     userwlock = B_FALSE;
  } else {
    error = B_TRUE;
  }

  /* Print usage statement and exit on error. */
  if (error) {
    fprintf (stderr, "Usage: \"%s rw\" or \"%s mutex\"\n", argv[0], argv[0]);
    exit (0);
  }

  /* Get starting reference time. */
  startingTime = times(&junk);

  /* Create threads. */
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


/*******************************************************************************
This is the main for the writer thread.  Get the appropriate lock, do the read
or write, and then release the lock.  Note that trying to get either lock can
force the thread to block if the lock is unavailable.
/******************************************************************************/
void * writerMain(void *arg) {

  int loopCount;

  for (loopCount = 0; loopCount < LOOP_MAX; loopCount++) {

    /* Get either the rwlock or the mutex, depending on the mode. */
    if (userwlock) {
      pthread_rwlock_wrlock(&rwlock);
    } else {
      pthread_mutex_lock(&mutex);
    }

    doWrite();

    /* Release the lock. */
    if (userwlock) {
      pthread_rwlock_unlock(&rwlock);
    } else {
      pthread_mutex_unlock(&mutex);
    }
  }
}


/*******************************************************************************
This is the main for the reader threads.  Get the locks as for the writer
threads, except specify that this is a reader and not a writer when getting the
multiple reader single writer lock.
/******************************************************************************/
void * readerMain(void * idArg) {

  int id = (int)idArg;	/* Arg passed is the thread's ID number. */

  int loopCount;

  for (loopCount = 0; loopCount < LOOP_MAX; loopCount++) {

    /* Get either the rwlock or the mutex, depending on the mode. */
    if (userwlock) {
      pthread_rwlock_rdlock(&rwlock);
    } else {
      pthread_mutex_lock(&mutex);
    }

    doRead();

    /* Release the lock. */
    if (userwlock) {
      pthread_rwlock_unlock(&rwlock);
    } else {
      pthread_mutex_unlock(&mutex);
    }
  }
}
