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

long get_sum(int* array,int size){
    long sum = 0;
    int i;
    for (i=0; i<size; sum+=array[i], ++i);
    return sum;
} 

int main(){
     int i,start;
     long sum = 0,partial_sum;
     int *array;

     array = (int*) malloc(sizeof(int) * N );
     for (i=0; i<N; ++i) array[i] = 1;
     printf("sum: %ld\n",get_sum(array,N));
     free(array);
     return 0;
}
