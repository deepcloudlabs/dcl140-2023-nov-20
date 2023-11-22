/* 
 * redirect_out.c   :   check for > 
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"

/*
 * Look for ">" in myArgv, then redirect output to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_out(char ** myArgv) {
  int i;
  int fd;

  /* search forward for > */
  /* Fill in code. */

  if (myArgv[i]) {	/* found ">" in vector. */

    /* Open file. */
    /* Fill in code. */

    /* Redirect to use it for output. */
    /* Fill in code. */

    /* Cleanup / close unneeded file descriptors. */
    /* Fill in code. */

    /* Remove the > and the filename from myArgv. */
    /* Fill in code. */
  }
  return 0;
}
