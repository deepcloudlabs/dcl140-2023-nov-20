#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

  pid_t  pid;

  pid = fork();

  switch(pid) {

  /* fork failed! */
  case -1: 
    perror("fork");
    exit(1);

  /* in new child process */
  case 0:
    execlp("ls", "ls", "-F", (char *)0);
    /* why no test? */ 
    perror("execlp"); 
    exit(1);

  /* in parent, pid is PID of child */
  default: 
    break;
  }

  /* Rest of parent program code */
  wait(NULL);
  printf("In parent, my child is %d\n", pid);
} 
