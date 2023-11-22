#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int do_child_stuff() ;

int main() {

  pid_t pid;

  pid = fork();

  switch(pid) {

  /* fork failed! */
  case -1: 
    perror("fork");
    exit(1);

  /* in new child process */
  case 0: 
    printf("In Child, my pid is: %d\n", getpid() );
    do_child_stuff();
    exit(0);

  /* in parent, pid is PID of child */
  default: 
    break;
  }

  /* Rest of parent program code */
  printf("In parent, my child is %d\n", pid);
} 

int do_child_stuff() {
  printf("\t Child activity here \n");
}
