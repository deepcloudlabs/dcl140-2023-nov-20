/* 
 * pipe_command.c  :  deal with pipes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "shell.h"
#include "pipe_command.h"

void pipe_and_exec(char **myArgv) {
  int pipe_argv_index = pipe_present(myArgv);
  int pipefds[2];

  switch (pipe_argv_index) {

    case -1:	/* Pipe at beginning or at end of argv;  See pipe_present(). */
      fputs ("Missing command next to pipe in commandline.\n", stderr);
      errno = EINVAL;	/* Note this is NOT shell exit. */
      break;

    case 0:	/* No pipe found in argv array.  See pipe_present(). */
      execvp(myArgv[0], myArgv);
      break;

    default:	/* Pipe in the middle of argv array.  See pipe_present(). */

      /*
       * Split arg vector into two where the pipe symbol was found.
       * Terminate first half of vector.
       */
      myArgv[pipe_argv_index] = NULL;

      pipe(pipefds);

      switch(fork()) {

        case -1 :
	  break;

        /* Talking parent.  Remember this is a child forked from shell. */
        default :
  
	  /* Redirect output of "parent" through the pipe. */
          dup2(pipefds[1],STDOUT_FILENO);

	  /* Don't need write side of pipe, and read side dup'ed to stdin. */
          close(pipefds[1]);
          close(pipefds[0]);

	  execvp(myArgv[0],myArgv);
	  break;

        /* Listening child. */
        case 0 :

	  /* Redirect input of "child" through pipe. */
          dup2(pipefds[0],STDIN_FILENO);

	  /* Don't need read side of pipe open.  Write side dup'ed to stdout. */
          close(pipefds[0]);
          close(pipefds[1]);

	  /* Exec and recursively deal with other pipes */
          pipe_and_exec(&myArgv[pipe_argv_index+1]);
      }
  }

  perror("Couldn't fork or exec child process");
  exit(errno);
}
