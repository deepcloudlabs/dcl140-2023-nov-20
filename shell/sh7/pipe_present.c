/*
 *  pipe_present.c :  check for | 
 */

#include <stdio.h>
#include <string.h>
#include "shell.h"

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char ** myCurrentArgv) {
  int index;

  for (index = 0; myCurrentArgv[index] != NULL; index++) {
    if (!strcmp(myCurrentArgv[index],"|")) {
      break;
    }
  }

  /* At the beginning or at the end. */
  if ((index == 0) ||
      ((myCurrentArgv[index] != NULL) && (myCurrentArgv[index+1] == NULL))) {
    return -1;

  /* Off the end. */
  } else if (myCurrentArgv[index] == NULL) {
    return 0;

  /* In the middle. */
  } else {
    return index;
  }
}
