/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/

/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_echo(char **argv) {
  int nlflag = 1;	/* Cleared upon finding "-n" in argv[1]. */
  int i;

  if ((argv[1]) && (!strcmp(argv[1], "-n"))) {
    nlflag = 0;
    argv++;
  }

  for (i = 1; argv[i] != NULL; i++) {
    fputs(argv[i], stdout);
    putchar(' ');
  }

  if (nlflag)
    putchar('\n');
}


/* quit/exit/logout/bye command. */
static void bi_quit(char **argv) {
  exit(0);
}


/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
  char * keyword;		/* When this field is argv[0] ... */
  void (* do_it)(char **);	/* ... this function is executed. */
} inbuilts[] = {
  { "echo", bi_echo },		/* List of (argv[0], function) pairs. */
  { "quit",bi_quit},
  { "exit",bi_quit},
  { "bye",bi_quit},
  { "logout",bi_quit},
  { NULL, NULL}			/* NULL terminated. */
};

/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; /* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd) {
  struct cmd *tableCommand;

  for (tableCommand = inbuilts ; tableCommand->keyword != NULL; tableCommand++)
    if (strcmp(tableCommand->keyword,cmd) == 0) {
      this = tableCommand;
      return 1;
    }
  return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv) {
  this->do_it(argv);
}
