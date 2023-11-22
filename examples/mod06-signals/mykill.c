#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char *argv[]) {
  int pid;
  sscanf(argv[1],"%d",&pid);
  printf("Killing the process %d...\n",pid);
  /* Send SIGUSR1 signal to my parent process */
  kill( pid, SIGKILL );
}
