#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdio.h>

void childhandler(int signo) {

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

    /* 3. status contains the reaped status of one child */
    /*    If desired, save status for main program. */
  }

  errno = saveerrno;
  return;
}
