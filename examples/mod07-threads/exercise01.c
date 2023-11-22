/* Compile as "cc -o mycreate mycreate.c -lpthread" */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#define NUM_THREADS 5
#define SLEEP_TIME 10

pthread_t tid[NUM_THREADS];	/* Array of thread IDs. */

void* task(void *);	/* Thread routine. */

void thread_start() {
  int i;

  /* Create and start threads. */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_create(&tid[i], NULL, task, NULL);
  }
}

void thread_wait() {
  int i;

  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(tid[i], NULL);
  }
}

void* task(void * arg) {
  fprintf(stderr,"Thread %li is working now...\n",pthread_self());   
  sleep(rand()%5+5);
  fprintf(stderr,"Thread %li is returning now...\n",pthread_self());   
}

int main() {
  thread_start();
  thread_wait();
}
