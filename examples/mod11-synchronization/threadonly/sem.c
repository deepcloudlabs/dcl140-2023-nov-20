/******************************************************************************
This program demonstrates the use of a semaphore to manage many instances of a
resource.  Here, the resource is a bank teller, and this semaphore manages five
of them.  The semaphore makes clients wait when there are no more tellers
available and the client chooses to, and generally keeps track of the number of
tellers available.
/*****************************************************************************/

#include <stdio.h>		/* Standard IO Library. */
#include <pthread.h>		/* Posix threads. */
#include <semaphore.h>		/* For semaphore definitions. */
#include <unistd.h>		/* For _SC_CLK_TCK. */
#include <stdlib.h>		/* For random functions. */
#include <errno.h>		/* Error definitions. */

#include "utils.h"		/* for fractSleep() and printWithTime(). */

#define THREADS_OF_THIS_PROCESS 0 /* sem_init flag; sem for this proc only. */

#define NUM_CLIENTS 25		/* Number of clients wanting to do business. */
#define NUM_TELLERS 5		/* Number of bank tellers. */
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

/* This is the semaphore which regulates the bank tellers.  It operates like a
mutex but can manage more than one resource. */
sem_t bankLine;

#if REPORT_NUM_SEM_FREE
pthread_mutex_t semMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t semCond  = PTHREAD_COND_INITIALIZER;
#endif

/* Forward references. */
void * clientMain(void *);
void doBusiness(int);


/******************************************************************************
Main program.  Initialize, start threads and wait for them to complete.
/*****************************************************************************/
int main(int argc, char *argv[]) {

  int count;

  /* Define threads. */
  pthread_t threads[NUM_CLIENTS];

  /* Initialize random number seed. */
  srand48(time(NULL));

  /* Initialize semaphore to manage NUM_TELLERS quantity of a resource.
  Set up so that only threads of this process can use it. */
  if (sem_init(&bankLine, THREADS_OF_THIS_PROCESS, NUM_TELLERS)) {
    perror ("Error initializing semaphore");
    exit (errno);
  }

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
}


/******************************************************************************
This is the main for each client thread.

Simulate a client.  Figure that about 30% will be in a hurry, and that these
clients will not wait for a teller if there is not one available.  Assume that
the rest will wait for one if necessary.
/*****************************************************************************/
void * clientMain(void *arg) {

  int whoAmI = (int)arg;	/* Get ID number from argument. */
  int availableTellers;		/* Stores number of available tellers.  */
  boolean_t inaHurry = B_FALSE;	/* True if this customer is in a hurry. */
  char string[80];		/* String used to format output. */

  /* Suppose 30% of clients are in a hurry. */
  inaHurry = (drand48() <= .30);

#if REPORT_NUM_SEM_FREE
  pthread_mutex_lock(&semMutex);
#endif

  /* This is one of those rushed clients. */
  if (inaHurry) {

    /* See if a teller is available.  If not, sem_trywait returns EAGAIN. */
    if ((sem_trywait(&bankLine) == -1) && (errno == EAGAIN)) {

#if REPORT_NUM_SEM_FREE
      pthread_mutex_unlock(&semMutex);
#endif

      sprintf (string,"Client %d is in a hurry and no tellers available.  Skipping.\n", whoAmI);
      printWithTime(string);
      return 0L;

    /* If get here, there is a teller available right away.  Drop thru and do
    business. */
    } else {

#if REPORT_NUM_SEM_FREE
      sem_getvalue(&bankLine, &availableTellers);
      pthread_mutex_unlock(&semMutex);
      sprintf (string,
	  "Client %d is in a hurry and got a teller. Count is now %d\n",
	  whoAmI, availableTellers);
      printWithTime(string);
#else  
      sprintf (string,
	  "Client %d is in a hurry and got a teller.\n", whoAmI);
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
 
    while ((sem_trywait(&bankLine) == -1) && (errno == EAGAIN)) {
      pthread_cond_wait(&semCond, &semMutex);
    }
    sem_getvalue(&bankLine, &availableTellers);
    sprintf (string,
	"Client %d got a teller.  Count is now %d\n", whoAmI, availableTellers);
    printWithTime(string);
    pthread_mutex_unlock(&semMutex);
 
#else

    sem_wait(&bankLine);	/* Wait for next available teller. */
    sprintf (string, "Client %d got a teller.\n", whoAmI);
    printWithTime(string);

#endif
  }

  /* We've got a teller.  Do our business. */
  doBusiness(whoAmI);

#if REPORT_NUM_SEM_FREE
  pthread_mutex_lock(&semMutex);

  /* Done doing business.  Teller is available. */
  sem_post(&bankLine);

  /* Get value of the semaphore to report back as a diagnostic. */
  sem_getvalue(&bankLine, &availableTellers);

  pthread_cond_signal(&semCond);
  pthread_mutex_unlock(&semMutex);

  sprintf (string,
      "Client %d done doing business.  Available teller count: %d\n",
      whoAmI, availableTellers);
  printWithTime(string);
#else

  /* Done doing business.  Teller is available. */
  sem_post(&bankLine);

  sprintf (string,
      "Client %d done doing business.\n", whoAmI);
  printWithTime(string);
#endif
}


/******************************************************************************
Simulate doing business by delaying for the time it takes to do business.
Each client takes a random amount of time.
/*****************************************************************************/
void doBusiness(int whoAmI) {
  char string[80];
  sprintf (string,"Client %d doing business\n",whoAmI);
  printWithTime(string);
  fractSleep(drand48() * BUSINESS_TIME_RANGE);
}
