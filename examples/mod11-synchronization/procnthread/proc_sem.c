/******************************************************************************
This program demonstrates the use of a semaphore to manage across many
processes many instances of a resource.  Here, the resource is a bank teller,
and this semaphore manages NUM_TELLERS of them.  The semaphore makes clients
wait when there are no more tellers available and the client chooses to, and
generally keeps track of the number of tellers available.
/*****************************************************************************/

#include <stdio.h>		/* Standard IO Library. */
#include <pthread.h>		/* For posix threads. */
#include <semaphore.h>		/* For semaphore definitions. */
#include <unistd.h>		/* For _SC_CLK_TCK. */
#include <stdlib.h>		/* For random functions. */
#include <fcntl.h>		/* For file open definitions. */
#include <sys/mman.h>		/* For mmap definitions. */
#include <errno.h>		/* Error definitions. */
#include <memory.h>

#include "utils.h"		/* For fractSleep() and printWithTime(). */

#define SHARED_DATA_FILE "/tmp/proc_sem.dat"

/* sem_init flag; semaphore for threads of multiple processes. */
#define INTERPROCESS_THREADS PTHREAD_PROCESS_SHARED

#define NUM_CLIENTS 25		/* Number of clients wanting to do business. */
#define NUM_TELLERS 4		/* Number of bank tellers. */
#define DELAY_BTWN_CLIENTS 0.3	/* Time between new clients (sec). */
#define BUSINESS_TIME_RANGE 4 /* Time/client range btwn 0 & this value (sec). */

#define STDOUT 1

typedef int boolean_t;
#define B_FALSE 0
#define B_TRUE 1

/* NOTE that there is a fine window between the sem_post() call (line 274) and
sem_getvalue() below it where another thread could sneak in and change the
value of the semaphore before it is read, since the two calls are not part of
an indivisible operation.  (No such window would exist if the value of the
semaphore were not needed externally (the normal case), as for reporting it,
for example.)

This window can be closed with a mutex, in effect making the sem_post() and
sem_getvalue() indivisible.  This introduces another problem, though, since
ALL calls to any semaphore operation need to be bracketed by the mutex,
including any calls to sem_wait() which can block indefinitely;  this could
cause deadlock.

The fix to prevent the deadlock situation lies in using sem_trywait() instead
of sem_wait() on line 248, and if the semaphore is unavailable to sleep on a
condition variable.  The thread is awakened via pthread_cond_signal() when
sem_post() is called.

REPORT_NUM_SEM_FREE compiles in the mutex and condition variable support if
reporting is requested.  Compare the code with and without this support, and
observe how the mutex and condition variable work together. */

#define REPORT_NUM_SEM_FREE 1	/* Turn on reporting of num of free sems.*/

/* Global stuff. */

typedef struct globalStuff {

  /* This is the semaphore which regulates the bank tellers.  It operates like
  a mutex but can manage more than one resource. */
  sem_t bankLine;
#if REPORT_NUM_SEM_FREE
  pthread_mutex_t semMutex;
  pthread_cond_t semCond;
#endif
} globalStuff;

/* Handle to shared area. */
struct globalStuff * globalArea;

/* True if this process initializes shared area. */
boolean_t createdHere = B_FALSE;

/* Forward references. */
void * clientMain(void *);
void doBusiness(int);


/*****************************************************************************/
int main(int argc, char *argv[]) {
/******************************************************************************
Main program.  Initialize shared semaphore, start threads and wait for them to
complete.
/*****************************************************************************/


  /* File handle to mmapped file. */
  int shared_fd;

  /* Define threads. */
  pthread_t threads[NUM_CLIENTS];

#if REPORT_NUM_SEM_FREE
  /* Define mutex and condition variable attributes. */
  pthread_mutexattr_t mutexAttributes;
  pthread_condattr_t condAttributes;
#endif

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
    perror ("Error opening shared semaphore area");
    exit (errno);
  }

  /* Expand the file, if we created it, to hold the shared data, before
  mmapping it. */
  if ((createdHere) && (ftruncate(shared_fd,sizeof(struct globalStuff)))) {
    perror ("ftruncate");
    exit (errno);
  }

  /* Mmap the file so all processes needing access to it can see it. */
  globalArea = (struct globalStuff *)mmap(
      NULL, sizeof(struct globalStuff), PROT_READ | PROT_WRITE, MAP_SHARED,
      shared_fd, 0);
  if (globalArea == (struct globalStuff *)-1) {
    perror ("Error getting virtual addr for shared semaphore.");
  exit (errno);
  }

  /* Initialize semaphore to manage NUM_TELLERS quantity of a resource if
  we created the semaphore.  Set up so that threads of any proc can use it. */
  if (createdHere) {
    memset(globalArea, 0,sizeof(struct globalStuff));
    if (sem_init(&globalArea->bankLine, INTERPROCESS_THREADS, NUM_TELLERS)) {
      perror ("Error initializing semaphore");
      exit (errno);
    }
#if REPORT_NUM_SEM_FREE
    if (pthread_mutexattr_init(&mutexAttributes)) {
      perror ("Error initializing mutex attributes");
      exit (errno);
    }
    /* Reflect in the attributes that this is an interprocess shared mutex. */
    if (pthread_mutexattr_setpshared(
	&mutexAttributes, PTHREAD_PROCESS_SHARED)) {
      perror ("pthread_mutexattr_setpshared");
      exit (errno);
    }
    if (pthread_mutex_init(&globalArea->semMutex, &mutexAttributes)) {
      perror ("Error initializing mutex");
      exit (errno);
    }
    if (pthread_condattr_init(&condAttributes)) {
      perror ("Error initializing condition variable attributes");
      exit (errno);
    }
    if (pthread_condattr_setpshared(
	&condAttributes, PTHREAD_PROCESS_SHARED)) {
      perror ("pthread_condattr_setpshared");
      exit (errno);
    }
    if (pthread_cond_init(&globalArea->semCond, &condAttributes)) {
      perror ("Error initializing condition variable");
      exit (errno);
    }
#endif
  }

  /* Initialize random number seed. */
  srand48(time(NULL));

  /* Start client threads.  Delay between starting new clients to simulate a
  real bank situation where people stagger in (as opposed to flock to the bank
  all at once. */
  for (count = 0; count < NUM_CLIENTS; count++) {
    if (pthread_create( &threads[count], NULL, clientMain, (void *)count)) {
      perror ("Error starting client threads");
      exit (errno);
    }
    fractSleep(DELAY_BTWN_CLIENTS);
  }

  /* Wait for all threads to finish. */
  for (count = 0; count < NUM_CLIENTS; count++) {
    pthread_join(threads[count], (void **)NULL);
  }
exit(0);
}


/*****************************************************************************/
void * clientMain(void *arg) {
/******************************************************************************
This is the main for each client thread.

Simulate a client.  Figure that about 30% will be in a hurry, and that these
clients will not wait for a teller if there is not one available.  Assume that
the rest will wait for one if necessary.
/*****************************************************************************/

  int whoAmI = (int)arg;	/* Get ID number from argument. */
  int availableTellers;		/* Stores number of available tellers.  */
  boolean_t inaHurry = B_FALSE;	/* True if this customer is in a hurry. */
  char string[80];		/* String used to format output. */

  /* Suppose 30% of clients are in a hurry. */
  inaHurry = (drand48() <= .30);

#if REPORT_NUM_SEM_FREE
  pthread_mutex_lock(&globalArea->semMutex);
#endif

  /* This is one of those rushed clients. */
  if (inaHurry) {

    /* See if a teller is available.  If not, sem_trywait returns EAGAIN. */
    if ((sem_trywait(&globalArea->bankLine) == -1) && (errno == EAGAIN)) {
#if REPORT_NUM_SEM_FREE
      pthread_mutex_unlock(&globalArea->semMutex);
#endif
      sprintf (string,
	  "Client %d%c is in a hurry and no tellers available.  Skipping.\n",
	  whoAmI, (createdHere ? 'A' : 'B'));
      printWithTime(string);
      return 0L;

    /* If get here, there is a teller available right away.  Drop thru and do
    business. */
    } else {

#if REPORT_NUM_SEM_FREE
      sem_getvalue(&globalArea->bankLine, &availableTellers);
      sprintf (string,
	  "Client %d%c is in a hurry and got a teller. Count is now %d\n",
	  whoAmI, (createdHere ? 'A' : 'B'), availableTellers);
      printWithTime(string);
      pthread_mutex_unlock(&globalArea->semMutex);
#else
      sprintf (string,
	  "Client %d%c is in a hurry and got a teller.\n",
	  whoAmI, (createdHere ? 'A' : 'B'));
      printWithTime(string);
#endif
    }

  /* Not in a hurry. */
  } else { 
    /* Wait for next available teller. */

#if REPORT_NUM_SEM_FREE

    /* Calling sem_trywait() in a loop until we get the lock, and sleeping on
    the condition variable if the lock is unavailable is necessary to prevent
    deadlock, since we have the mutex. */

    while ((sem_trywait(&globalArea->bankLine) == -1) && (errno == EAGAIN)) {
      pthread_cond_wait(&globalArea->semCond, &globalArea->semMutex);
    }
    sem_getvalue(&globalArea->bankLine, &availableTellers);
    sprintf (string,
	"Client %d%c got a teller.  Count is now %d\n",
	whoAmI, (createdHere ? 'A' : 'B'), availableTellers);
    printWithTime(string);
    pthread_mutex_unlock(&globalArea->semMutex);

#else

    sem_wait(&globalArea->bankLine);
    sprintf (string,
	"Client %d%c got a teller.\n", whoAmI, (createdHere ? 'A' : 'B'));
    printWithTime(string);
#endif
  }

  /* We've got a teller.  Do our business. */
  doBusiness(whoAmI);

#if REPORT_NUM_SEM_FREE
  pthread_mutex_lock(&globalArea->semMutex);

  /* Done doing business.  Teller is available. */
  sem_post(&globalArea->bankLine);

  /* Get value of the semaphore to report back as a diagnostic. */
  sem_getvalue(&globalArea->bankLine, &availableTellers);

  pthread_cond_signal(&globalArea->semCond);
  pthread_mutex_unlock(&globalArea->semMutex);

  sprintf (string,
      "Client %d%c done doing business.  Available teller count: %d\n",
      whoAmI, (createdHere ? 'A' : 'B'), availableTellers);
  printWithTime(string);
#else

  /* Done doing business.  Teller is available. */
  sem_post(&globalArea->bankLine);

  sprintf (string,
      "Client %d%c done doing business.\n", whoAmI, (createdHere ? 'A' : 'B'));
  printWithTime(string);
#endif
}


/******************************************************************************
Simulate doing business by delaying for the time it takes to do business.
Each client takes a random amount of time.
/*****************************************************************************/
void doBusiness(int whoAmI) {
  char string[80];

  sprintf (string,"Client %d%c doing business\n",
      whoAmI, (createdHere ? 'A' : 'B'));
  printWithTime(string);
  fractSleep(drand48() * BUSINESS_TIME_RANGE);
}
