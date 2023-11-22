/* 
 * redirect_in.c  :  check for <
 */

#include <unistd.h>
#include <fcntl.h>
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
  /* Fill in code. */

  if (myArgv[i]) {	/* found "<" in vector. */

    /* Open file. */
    /* Fill in code. */

    /* Redirect stdin to use file for input. */
    /* Fill in code. */

    /* Cleanup / close unneeded file descriptors. */
    /* Fill in code. */

    /* Remove the < and the filename from myArgv. */
    /* Fill in code. */
  }
  return 0;
}
