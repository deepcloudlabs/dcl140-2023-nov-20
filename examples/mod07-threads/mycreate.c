/* Compile as "cc -o mycreate mycreate.c -lpthread" */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#define NUM_THREADS 5
#define SLEEP_TIME 10

pthread_t tid[NUM_THREADS];	/* Array of thread IDs. */

void* sleeping(void *);	/* Thread routine. */

void thread_start() {
  int i;

  /* Create and start threads. */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_create(&tid[i], NULL, sleeping, (void *)(SLEEP_TIME));
  }
}

void thread_wait() {
  int i;
  long return_value;

  /* Wait for threads to complete before exiting.  Otherwise, child threads are
  terminated if main completes first. */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(tid[i], (void **)&return_value);
    printf ("Thread %lx returns %li.\n",tid[i], return_value);
  }
}

/* Thread's "main".  Notice the generic arg and return type. */
void* sleeping(void * arg) {
  int sleep_time = (int)arg;
  printf("%d\n",sleep_time);
  sleep(sleep_time);
  return ((void *)sleep_time);
}

int main() {
  thread_start();
  thread_wait();
}
