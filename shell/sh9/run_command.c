/* 
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include "pipe_command.h"
#include "run_command.h"
#include "shell.h"

/*
 * Note this is the vanilla non-timer solution.
 * Fill in code to add the features for timer processing.
 */

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

	display_status(pid, stat);
      }
      return;

    /* Child. */
    case 0 :
      signal(SIGINT, SIG_DFL);
      signal(SIGQUIT, SIG_DFL);
      if (redirect_in(myArgv)) {	/* Redirect input and update argv. */
	fputs ("Missing input file.", stderr);
	exit (errno);
      }
      if (redirect_out(myArgv)) {	/* Redirect output and update argv. */
	fputs ("Missing output file.", stderr);
	exit (errno);
      }
      pipe_and_exec(myArgv);
      exit(errno);
   }
}

void display_status(pid_t pid, int stat) {
  if (WIFEXITED(stat) && WEXITSTATUS(stat)) {
    fprintf(stderr, "Child %d exited with error status %d: %s\n",
	pid, WEXITSTATUS(stat), strerror(WEXITSTATUS(stat)));

  } else if (WIFSIGNALED(stat) && WTERMSIG(stat)) {
    fprintf (stderr, "Child %d exited due to signal %d: %s\n",
	pid, WTERMSIG(stat), strsignal(WTERMSIG(stat)));
  }
}
