#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 80

int main() {

  int pd, n = 0;
  char inmsg[BUFSIZE];
  char* introMessage = "Listener heard:"; 

  /* open the pipe for read */
  pd = open("./FIFO1", O_RDONLY);
  if(pd == -1) {
    perror("open");
    exit(1);
  }

  write(STDOUT_FILENO, introMessage, strlen(introMessage));

  while( (n = read(pd, inmsg, BUFSIZE ) ) > 0 ) {
    write(STDOUT_FILENO, inmsg, n );
  }

  if( n == -1 ) {
    perror("read");
    exit(1);
  }

  write(STDOUT_FILENO,"\n", 1 );
  close(pd);
  return(0);
}
