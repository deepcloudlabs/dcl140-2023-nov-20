/*
 * shell.c  : test harness for parse routine
 */

#define LONGLINE 255

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "shell.h"

void handler(int signal_no){
  fputs("Do not send interrupt!\n",stdout);
  fputs("myshell -> ",stdout);
}

int main() {
  char line[LONGLINE];
  int i;
  char **myArgv;
  struct sigaction child_handling;

  /* Ignore ^C and ^\ in shell, so they are passed on to child processes. */
  /*
  struct sigaction act;
  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGINT);
  sigaddset(&act.sa_mask, SIGQUIT);
  act.sa_flags = SA_RESTART;
  sigaction(SIGQUIT, &act,(struct sigaction *) NULL);
  sigaction(SIGINT, &act,(struct sigaction *) NULL);
  */
  sigset(SIGINT, SIG_IGN);
  sigset(SIGQUIT, SIG_IGN);
  /* Optional exercise: Set up to reap children run in the background. */
  child_handling.sa_handler = child_handler;
  child_handling.sa_flags = SA_RESTART;
  sigfillset(&child_handling.sa_mask);
  sigaction(SIGCHLD, &child_handling,(struct sigaction *) NULL);
  
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
