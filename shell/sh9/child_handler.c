#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "run_command.h"

void child_handler(int signo) {

  int    status, saveerrno;
  pid_t  pid;
  extern int errno;

  saveerrno = errno;

  /* get all outstanding terminated children */
  for (;;) { 

    pid = waitpid(-1, &status, WNOHANG);

    if (pid == 0) {
      /* 1. no dead children, but some live ones */
      break;

    } else if (pid == -1 && errno == ECHILD) {
      /* 2. no more children, dead or running */
      break;

    } else if (pid == -1) {
      /* should not get this */
      perror("waitpid");
      abort();
    }

    display_status(pid,status);
  }

  errno = saveerrno;
  return;
}
