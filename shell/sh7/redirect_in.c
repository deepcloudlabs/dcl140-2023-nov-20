/* 
 * redirect_in.c  :  check for <
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shell.h"

/*
 * Look for "<" in myArgv, then redirect input to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_in(char ** myArgv) {
  int i;
  int fd;

  /* search forward for < */
  for (i = 0; myArgv[i] != NULL; i++) {
    if (!strcmp(myArgv[i], "<")) {
      break;
    }
  }

  if (myArgv[i]) {	/* found "<" in vector. */

    if (!myArgv[i+1]) {	/* File for redirection has not been provided. */
      errno = EINVAL;
      return -1;

    } else {		/* Open file. */
      if ((fd =open(myArgv[i+1],O_RDONLY)) == -1) {
        perror(myArgv[i+1]);
        return -1;
      }
    }

    dup2(fd,STDIN_FILENO);	/* Redirect to use it for input. */
    close(fd);

    for (i = i + 2; myArgv[i] != NULL; i++) { /* close up the vector */
      myArgv[i-2] = myArgv[i];
    }
    myArgv[i-2] = NULL;      /* NULL pointer at end */
  }
  return 0;
}
