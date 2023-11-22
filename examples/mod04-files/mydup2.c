#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

  int fd;

  fd = open("/tmp/foo", O_CREAT|O_WRONLY|O_TRUNC, 0660);

  if( fd == -1 ) { 
    perror("open"); 
    exit(1);
  }

  /* makefile desc 1 refer to "/tmp/foo" */
  if( dup2(fd, STDOUT_FILENO) == -1 ) { 
    perror("dup2"); 
    exit(1); 
  }

  /* Don't need fd any more */
  close(fd);    

  /* stdout output goes to "/tmp/foo" */
  printf("Hi Folks!\n");
}
