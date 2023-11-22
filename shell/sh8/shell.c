/*
 * shell.c  : test harness for parse routine
 */

#define LONGLINE 255

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "shell.h"

int main() {
  char line[LONGLINE];
  int i;
  char **myArgv;
  struct sigaction child_handling;

  /* Ignore ^C and ^\ in shell, so they are passed on to child processes. */
  /* Fill in code. */

  /* Optional exercise: Set up to reap children run in the background. */
  /* Fill in code. */

  fputs("myshell -> ",stdout);
  while (fgets(line,LONGLINE,stdin)) {

    /* Create argv array based on commandline. */
    if (myArgv = parse(line)) {

      /* If command is recognized as a builtin, do it. */
      if (is_builtin(myArgv[0])) {
        do_builtin(myArgv);

      /* Non-builtin command. */
      } else {
	run_command(myArgv);
      }

      /* Free argv array. */
      free_argv(myArgv);
    }

    fputs("myshell -> ",stdout);
  }
  exit(0);
}
