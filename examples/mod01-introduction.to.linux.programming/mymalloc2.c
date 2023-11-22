#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

int *q= (int *) malloc(256*sizeof(int));
int *p= (int *) malloc(128*sizeof(int));
printf("p=%x\n",p);
printf("q=%x\n",q);
p= (int *) realloc(p,256*sizeof(int));
printf("p=%x\n",p);
printf("q=%x\n",q);
free(p);
free(q);

  return 0;
}
