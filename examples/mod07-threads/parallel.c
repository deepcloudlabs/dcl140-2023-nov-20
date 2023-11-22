#define _GNU_SOURCE

#define N 400000000

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct task_data {
    int *array;
    int start;
    int size;
};

typedef struct task_data task_data_t;

void* get_sum(void* task){
    static int core=0;
    long sum = 0;
    int i,k,size;
    int n = core++;
    //printf("setting affinity: %d\n",n);
    // setting cpu affinity
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(n,&cpuset);
    pthread_t current_thread = pthread_self();
    pthread_setaffinity_np(current_thread,sizeof(cpu_set_t),&cpuset);
    // solving problem
    task_data_t *td = (task_data_t*) task;
    size=td->size;
    //fprintf(stderr,"task: size: %d, start: %d\n",size,td->start);
    for (i=0,k=td->start;i<size;sum+=td->array[k],++k,++i);
    return (void*) sum;
} 

int main(){
     int i,start;
     long sum = 0,partial_sum;
     int *array;

     int number_of_cores = sysconf(_SC_NPROCESSORS_ONLN);
     int task_data_size = N / number_of_cores;
     task_data_t *tasks = (task_data_t*) malloc(sizeof(task_data_t) * number_of_cores);
     array = (int*) malloc(sizeof(int) * N );
     for (i=0;i<N;++i) array[i] = 1;
     printf("cores: %d\n",number_of_cores);
     pthread_t *threads = (pthread_t *) malloc(number_of_cores * sizeof(pthread_t));
     for (start=0,i=0; i<number_of_cores; ++i, start += task_data_size){
         tasks[i].array=array;
         tasks[i].size=task_data_size;
         tasks[i].start=start;
         pthread_create(threads+i, NULL, get_sum, (void*)(tasks+i));
     }
     for (i=0; i<number_of_cores; ++i){
         pthread_join(threads[i], (void**) &partial_sum);
         //printf("partial sum: %ld\n",partial_sum);
         sum += partial_sum;
     }
     printf("sum: %ld\n",sum);
     free(threads);
     free(array);
     free(tasks);
     return 0;
}
