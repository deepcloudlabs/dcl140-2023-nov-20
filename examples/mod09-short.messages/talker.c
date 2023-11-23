#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  int pd, n,i;
  char msg[] = "Hi, Folks!";

  printf("Talker's here.\n");

  /* open the pipe for write */
  pd = open("./FIFO1", O_WRONLY);
  if( pd == -1) {
    perror("open");
    exit(1);
  }
  for (i=0;i<10;++i){
  n = write(pd, msg, strlen(msg) + 1 );
  if( n == -1) {
    perror("write");
    exit(1);
  }
  }

  close(pd);
  return(0);
}
