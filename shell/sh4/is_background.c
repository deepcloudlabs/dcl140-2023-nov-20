/* 
 * is_background.c :  check for & at end
 */

#include <stdio.h>
#include "shell.h"

int is_background(char ** myArgv) {

  if (*myArgv == NULL)
    return FALSE;

  /* Look for "&" in myArgv, and process it. */
  while (*myArgv != NULL) myArgv++;  

  /* Return TRUE if found. */
  --myArgv;
  if(*myArgv[0] == '&'){
      *myArgv = NULL;
      return TRUE;
  }
  return FALSE; 
  /* Fill in code. */
}
