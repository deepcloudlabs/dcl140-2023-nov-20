/*******************************************************************************
This module exemplifies a threadsafe stack which can be shared among multiple
processes as well as multiple threads.  It uses a mutex to coordinate access to
shared data.  It also demonstrates the use of condition variables by which
threads wait for the shared data to change, and by which threads notify/resume
each other when the state has changed.
/******************************************************************************/

#include <stdio.h>		/* For basic definitions. */
#include <pthread.h>		/* Posix threads. */
#include <fcntl.h>		/* For file open flags. */
#include <sys/mman.h>		/* For mmap definitions. */
#include <errno.h>		/* For error definitions. */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "utils.h"		/* For printWithTime(). */
#include "proc_stack.h"		/* Stack function definitions. */

/* Mmapped file containing the shared data. */
#define SHARED_DATA_FILE "/tmp/proc_stack.dat"

#define STACK_SIZE 3	/* Define a small stack size to cause contention. */

/* Define the data structure shared between the threads. */

typedef struct stackType {
    char buffer[STACK_SIZE];	/* Stack's buffer. */
    int index;			/* Stack's index. */
    pthread_mutex_t mutex;	/* Mutex lock for exclusive data access. */
    pthread_cond_t conditionVar;/* Condition var for coordinating threads. */
} stackType;

/* Handle to mmapped shared area. */
struct stackType *stack;

typedef int boolean_t;
#define B_FALSE 0
#define B_TRUE 1

/******************************************************************************/
void stackInit() {
/*******************************************************************************
Create shared mmapped region if necessary, then map to it.  If creating the
shared region, then initialize the locks needed to protect the stack, and
create and initialize the stack to be the size given.
/******************************************************************************/

  /* File handle to mmapped file. */
  int shared_fd;

  /* Attributes for initializing locks as interprocess. */
  pthread_mutexattr_t mutex_attributes;
  pthread_condattr_t cond_attributes;

  /* True if this process initializes shared area. */
  boolean_t createdHere = B_FALSE;

  /* Open the shared area.  Err out if it already exists (which means we are
  not the first process to use it, and thus not to be the creator if it. */
  shared_fd = open(SHARED_DATA_FILE, (O_RDWR | O_CREAT | O_EXCL), 0777);

  /* Flag that we are the creator if we are, or just open the file otherwise.*/ 
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
  if (ftruncate(shared_fd,sizeof(struct stackType))) {
    perror ("ftruncate");
    exit (errno);
  }

  /* Mmap the file so all processes needing access to it can see it. */
  stack = (struct stackType *)mmap(
      NULL, sizeof(struct stackType), PROT_READ | PROT_WRITE, MAP_SHARED,
      shared_fd, 0);
  if (stack == (struct stackType *)-1) {
    perror ("Error getting virtual addr for shared stack");
  exit (errno);
  }

  /* Initialize locks in shared region if we are the shared region creator. */
  if (createdHere) {

    /* Initialize mutex to be sharable among threads of different processes. */
    if (pthread_mutexattr_init(&mutex_attributes)) {
      perror ("pthread_mutexattr_init");
      exit (errno);
    }
    if (pthread_mutexattr_setpshared(
	&mutex_attributes, PTHREAD_PROCESS_SHARED)) {
      perror ("pthread_mutexattr_setpshared");
      exit (errno);
    }
    if (pthread_mutex_init(&stack->mutex, &mutex_attributes)) {
      perror ("pthread_mutex_init");
      exit (errno);
    }
  
    /* Initialize condition variable the same way as mutex. */
    if (pthread_condattr_init(&cond_attributes)) {
      perror ("pthread_condattr_init");
      exit (errno);
    }
    if (pthread_condattr_setpshared(
	&cond_attributes, PTHREAD_PROCESS_SHARED)) {
      perror ("pthread_condattr_setpshared");
      exit (errno);
    }
    if (pthread_cond_init(&stack->conditionVar, &cond_attributes)) {
      perror ("pthread_cond_init");
      exit (errno);
    }

    /* Initialize the stack index to refer to an empty stack. */
    stack->index = 0;
  }
}


/******************************************************************************/
void push(char oneChar) {
/*******************************************************************************
Push a character onto the stack.  Return in the second argument the stack index
corresponding to where the character is pushed.
/******************************************************************************/
  char string[25];

  /* Get the lock before accessing the shared data in any way. */
  pthread_mutex_lock(&stack->mutex);

  /* Test the data while under mutex protection, to see if the stack is
  pushable.  Don't change the data yet as the mutex lock will be given up if
  pthread_cond_wait() is called;  a free lock implies consistent data. */
  while (stack->index == STACK_SIZE) {
    printWithTime("Push sleeping...\n");
    pthread_cond_wait(&stack->conditionVar, &stack->mutex);
  }

  /* Stack is pushable.  Push the data. */
  stack->buffer[stack->index++] = oneChar;

  sprintf (string,"Pushed:\tchar %c\tindex %d\n", oneChar, stack->index-1);
  printWithTime(string);

  /* Notify waiting threads (poppers in this case) that the stack has changed
  and, due to the operation just completed, there is now something to pop. */
  pthread_cond_signal(&stack->conditionVar);

  /* Release the mutex.  All done with shared data access. */
  pthread_mutex_unlock(&stack->mutex);
}

/******************************************************************************/
char pop(int *indexArg) {
/*******************************************************************************
Pop a character from the stack.  Return in the second argument the stack index
corresponding to where the character is popped.
/******************************************************************************/
  char toReturn;
  char string[25];

  /* Get the lock before accessing the shared data in any way. */
  pthread_mutex_lock(&stack->mutex);

  /* Test the data while under mutex protection, to see if the stack is
  pushable.  Don't change the data yet as the mutex lock will be given up if
  pthread_cond_wait() is called;  a free lock implies consistent data. */

  while (stack->index == 0) {
    printWithTime("Pop sleeping...\n");
    pthread_cond_wait(&stack->conditionVar, &stack->mutex);
  }

  /* Stack is poppable.  Pop the data. */
  toReturn = stack->buffer[--(stack->index)];

  sprintf (string, "Pop:\tchar %c\tindex %d\n", toReturn, stack->index);
  printWithTime(string);

  /* Notify waiting threads (pushers in this case) that the stack has changed
  and, due to the operation just completed, there is now something to push. */
  pthread_cond_signal(&stack->conditionVar);

  /* Release the mutex.  All done with shared data access. */
  pthread_mutex_unlock(&stack->mutex);

  return toReturn;
}
