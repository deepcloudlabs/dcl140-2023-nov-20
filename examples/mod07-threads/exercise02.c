#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int cores;

void* task(void *);	/* Thread routine. */

void* task(void * arg) {
    static int core_id = 0;
    core_id++;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id%cores,&cpuset);
    pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&cpuset);
    fprintf(stderr,"Thread %li is working now...\n",pthread_self());   
    sleep(rand()%60+60);
    fprintf(stderr,"Thread %li is returning now...\n",pthread_self());   
}

int main() {
    int i;
    cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t tid[cores];	/* Array of thread IDs. */
    for (i = 0; i < cores; i++) {
      pthread_create(&tid[i], NULL, task, NULL);
    }
    for (i = 0; i < cores; i++) {
      pthread_join(tid[i], NULL);
    }
    fprintf(stderr,"number of logical processors is %d\n",cores);
    return 0;
}
