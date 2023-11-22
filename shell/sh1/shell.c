/*
 * shell.c  : test harness for parse routine
 */

#define LONGLINE 255

#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

int main() {
  char line[LONGLINE];
  int i;
  char **myArgv;

  fputs("myshell -> ",stdout);

  /* Loop as long as there is something on the commandline. */
  while (fgets(line,LONGLINE,stdin)) {

    /* Parse the commandline. */
    if (myArgv = parse(line)) {

      /* Dump out the argv array. */
      for (i = 0; myArgv[i] != NULL; i++) {
        printf("[%d] : %s\n",i, myArgv[i]);
      }

      /* Free memory of the argv array. */
      free_argv(myArgv);
    }

    fputs("myshell -> ",stdout);
  }
  exit(0);
  return 0;
}
