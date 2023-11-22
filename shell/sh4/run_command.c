/* 
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include "shell.h"

/*
 * Note this is the vanilla non-background processing solution.
 * Fill in code to add the features for background processing.
 */

void run_command(char **myArgv) {
  pid_t pid;
  int stat;

  switch(pid = fork()) {

    /* Error. */
    case -1 :
      perror("fork");
      exit(errno);

    /* Parent. */
    default :
      waitpid(pid,&stat,0);	/* Wait for child to terminate. */

      if (WIFEXITED(stat) && WEXITSTATUS(stat)) {
        fprintf(stderr, "Child %d exited with error status %d: %s\n",
	    pid, WEXITSTATUS(stat), strerror(WEXITSTATUS(stat)));

      } else if (WIFSIGNALED(stat) && WTERMSIG(stat)) {
	fprintf (stderr, "Child %d exited due to signal %d: %s\n",
	    pid, WTERMSIG(stat), strsignal(WTERMSIG(stat)));
      }
      return;

    /* Child. */
    case 0 :
      execvp(*myArgv,myArgv);		/* Run command in child process. */
      perror(myArgv[0]);		/* No successful return from exec */
      exit(errno);
   }
}
