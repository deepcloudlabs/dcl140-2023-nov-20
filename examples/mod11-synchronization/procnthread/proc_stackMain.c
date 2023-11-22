/*******************************************************************************
This module implements the framework for a producer/consumer problem in which
separate threads push and pop data off a stack (module stack.c).
/******************************************************************************/

#include <stdio.h>		/* Standard IO Library. */
#include <pthread.h>		/* Posix threads. */
#include <errno.h>		/* Error definitions. */
#include <stdlib.h>		/* For random functions. */

#include "proc_stack.h"		/* Stack definitions. */
#include "utils.h"		/* for fractSleep() and printWithTime(). */

#define MAX_COUNT 25		/* Maximum iteration count. */
#define NUM_LETTERS 26		/* Number letters in alphabet. */

/* Forward references. */
void* producer(void *);
void* consumer(void *);
char getRandomChar();

/******************************************************************************/
int main(int argc, char *argv[]) {
/*******************************************************************************
Main program.  Initialize and spawn threads.
/******************************************************************************/

  /* Define one producer thread and one consumer thread. */
  pthread_t producerThread;
  pthread_t consumerThread;

  /* Initialize seed for random number generator based on time in seconds. */
  srand48(time(NULL));

  /* Initialize the stack.  NOTE: This line is the only difference between
  this and the threadonly version of stackMain. */
  stackInit();

  /* Create threads. */
  if (pthread_create( &producerThread, NULL, producer, NULL)) {
    perror ("Error starting producer thread");
    exit (errno);
  }

  if (pthread_create( &consumerThread, NULL, consumer, NULL)) {
    perror ("Error starting consumer thread");
    exit (errno);
  }

  /* Wait for threads to finish. */
  pthread_join(producerThread, (void **)NULL);
  pthread_join(consumerThread, (void **)NULL);
}


/******************************************************************************/
void* producer(void *arg) {
/*******************************************************************************
Producer thread main.
/******************************************************************************/
  int count;
  char oneChar;

  /* Push a random character on the stack, then delay for random amt of time. */

  for (count = 0; count < MAX_COUNT; count++) {
    oneChar = getRandomChar();	/* Get a random character. */
    push(oneChar);		/* Push it on the stack. */
    fractSleep(drand48());	/* Sleep between 0 and 1 sec. */
  }
}


/******************************************************************************/
void* consumer(void *arg) {
/*******************************************************************************
Consumer thread main.
/******************************************************************************/
  int count;
  char oneChar;

  /* Pop a character from the stack, then delay for random amt of time. */

  for (count = 0; count < MAX_COUNT; count++) {
    oneChar = pop();		/* Pop a character. */
    fractSleep(drand48());	/* Sleep between 0 and 1 sec. */
  }
}


/******************************************************************************/
char getRandomChar() {
/*******************************************************************************
Return a random character from "A" to "Z"
/******************************************************************************/
  /* Get number in range 0 - 25 */
  int intChar = drand48() * NUM_LETTERS;

  /* Return a capital letter. */
  return ('A' + (char)intChar);
}
