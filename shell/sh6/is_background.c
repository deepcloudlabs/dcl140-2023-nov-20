/* 
 * is_background.c :  check for & at end
 */

#include <stdio.h>
#include "shell.h"

int is_background(char ** myArgv) {

  if (*myArgv == NULL)
    return 0;

   while (*myArgv)      /* Go to end of vector */
      myArgv++;

   if (**--myArgv == '&') {  /* search backwards for & */
     *myArgv = NULL;		/* Remove from vector. */
     return TRUE;		/* Note that it was found. */
   }

   return FALSE;
}
