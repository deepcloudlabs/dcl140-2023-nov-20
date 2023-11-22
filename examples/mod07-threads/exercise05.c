#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int cores;
int state = 0;
pthread_spinlock_t sp_lock;

void* task(void *);	/* Thread routine. */

void* task(void * arg) {
    int i;
//    fprintf(stderr,"Thread %li is working now...\n",pthread_self());   
    pthread_spin_lock(&sp_lock);
    for (i=0;i<1000000;++i){
        state++;
    }
    pthread_spin_unlock(&sp_lock);
//    printf(stderr,"Thread %li is returning now...\n",pthread_self());   
}

int main() {
    int i;
    pthread_spin_init(&sp_lock,PTHREAD_PROCESS_PRIVATE);
    cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t tid[cores];	
    for (i = 0; i < cores; i++) {
      pthread_create(&tid[i], NULL, task, NULL);
    }
    for (i = 0; i < cores; i++) {
      pthread_join(tid[i], NULL);
    }
    fprintf(stderr,"\nstate: %d\n",state);
    return 0;
}
