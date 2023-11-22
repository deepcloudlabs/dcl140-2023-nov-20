/*******************************************************************************
This program demonstrates the use of a posix thread mutex shared between
multiple processes to coordinate access by multiple processes to a shared
resource.  A common counter is the shared resource.
/******************************************************************************/
#include <sys/types.h>		/* For general. */
#include <stdio.h>		/* Standard IO Library. */
#include <pthread.h>		/* Posix threads. */
#include <sys/mman.h>		/* Mmap stuff. */
#include <fcntl.h>		/* File opening attributes. */
#include <sys/signal.h>		/* For cleanup handling. */
#include <errno.h>		/* Error definitions. */
#include <stdlib.h>	
#include <unistd.h>

/* Shared data is created in this mmapped file. */
#define SHARED_DATA_FILE "/tmp/proc_mutex.dat"
#define B_TRUE 0
#define B_FALSE 1
/* Global stuff. */

typedef struct globalStuff {
  pthread_mutex_t sharedMutex;
  int count;
} globalStuff;

/* Handle to shared area. */
struct globalStuff * globalArea;

typedef int boolean_t;

/******************************************************************************/
void cleanup(int x) {
/*******************************************************************************
Cleanup handler, called as a signal handler for ^C.
Unlock the mutex before terminating.
/******************************************************************************/
  printf ("Cleaning up...\n");
  pthread_mutex_unlock(&globalArea->sharedMutex);
  exit(0);
}


/******************************************************************************/
int main() {
/*******************************************************************************
Main program.  Open shared area.  If it has not been opened before then set up
the shared resources there.  Finally, access the shared resource in a loop.
/******************************************************************************/

  int shared_fd;			/* Mmapped file of shared data. */
  pthread_mutexattr_t mutex_attributes;	/* For interprocess mutex init. */
  boolean_t createdHere = B_FALSE;	/* True if we set up shared area. */
  int delayTime;			/* Delay between accesses. */
  int temporary_storage;		/* Common counter storage. */

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
    perror ("Error opening shared mutex area");
    exit (errno);
  }

  /* Expand the file to hold the shared data, before mmapping it. */
  if (createdHere) {
    if (ftruncate(shared_fd,sizeof(struct globalStuff))) {
      perror ("ftruncate");
      exit (errno);
    }
  }

  /* Mmap the file so all processes needing access to it can see it. */
  globalArea = (struct globalStuff *)mmap(
      NULL, sizeof(struct globalStuff), PROT_READ | PROT_WRITE, MAP_SHARED,
      shared_fd, 0);
  if (globalArea == (struct globalStuff *)-1) {
    perror ("Error getting virtual addr for sharedMutex");
  exit (errno);
  }

  /* Set up cleanup handler for ^C. */
  signal(SIGINT, cleanup);

  /* Initialize mutex in shared region if we are the shared region creator. */
  if (createdHere) {

    /* Initialize mutex attributes list. */
    if (pthread_mutexattr_init(&mutex_attributes)) {
      perror ("pthread_mutexattr_init");
      exit (errno);
    }

    /* Reflect in the attributes that this is an interprocess shared mutex. */
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

    /* Specify different delays depending on whether or not this process
    created the shared region.  This will cause contention to show the mutex
    really working. */

    delayTime = 1;
  } else {
    delayTime = 2;
  }

  /* At this point the shared region is set up, and this process is connected
  to it. */

  while (1) {

    /* Wait for the lock. */
    printf ("Waiting for lock..."); fflush(stdout);

    if (pthread_mutex_lock(&globalArea->sharedMutex)) {
      perror ("pthread_mutex_lock");
      exit(errno);
    }

    /* Print a message and increment the common counter. */
    printf ("Got it!  Count incremented from %d", globalArea->count);
    fflush (stdout);
    temporary_storage = globalArea->count + 1;

    /* Hold the lock for a while when get it. */
    sleep (delayTime);

    /* Store and print new shared value. */
    globalArea->count = temporary_storage;
    printf (" to %d\n", globalArea->count);

    /* Release the lock. */
    printf ("Releasing lock...\n");

    if (pthread_mutex_unlock(&globalArea->sharedMutex)) {
      perror ("pthread_mutex_unlock");
      exit(errno);
    }

    /* Wait for 1 second. */
    printf ("Pausing for 1 second.\n");
    sleep (1);
  }
}
