/* 
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include "shell.h"

void run_command(char **myArgv) {
  pid_t pid;
  int stat;
  int run_in_background;

  /*
   * Check for background processing.
   * Do this before fork() as the "&" is removed from the argv array
   * as a side effect.
   */
  run_in_background = is_background(myArgv);

  switch(pid = fork()) {

    /* Error. */
    case -1 :
      perror("fork");
      exit(errno);

    /* Parent. */
    default :
      if (!run_in_background) {
        waitpid(pid,&stat,0);	/* Wait for child to terminate. */

        if (WIFEXITED(stat) && WEXITSTATUS(stat)) {
          fprintf(stderr, "Child %d exited with error status %d: %s\n",
	      pid, WEXITSTATUS(stat), strerror(WEXITSTATUS(stat)));

        } else if (WIFSIGNALED(stat) && WTERMSIG(stat)) {
	  fprintf (stderr, "Child %d exited due to signal %d: %s\n",
	      pid, WTERMSIG(stat), strsignal(WTERMSIG(stat)));
        }
      }
      return;

    /* Child. */
    case 0 :

      /* Redirect input and update argv. */
      if(redirect_in(myArgv)){
          fprintf(stderr,"Missing input file");
          exit(errno);
      }

      /* Redirect output and update argv. */
      if(redirect_out(myArgv)){
          fprintf(stderr,"Missing output file");
          exit(errno);
      }
      execvp(*myArgv,myArgv);		/* Run command in child process. */
      perror(myArgv[0]);		/* No successful return from exec */
      exit(errno);
   }
}
