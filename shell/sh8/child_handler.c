#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void child_handler(int signo) {
    pid_t pid;
    int status;

   for(;;){
      pid = waitpid(-1,&status,WNOHANG);
      if (pid == 0) break;
      else if(pid == -1 && errno == ECHILD) break;
      fputs("child process has died!\n",stdout);
      display_status(pid,status);
   }

}
