#include <sys/types.h> 
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#define DIE(x) perror(x),exit(1) 

int main() { 

  if(mknod("FIFO1",S_IFIFO | 0666,0) == -1) 
    DIE("FIFO1"); 

  if(mkfifo("FIFO2",0666) == -1) 
    DIE("FIFO2"); 
}
