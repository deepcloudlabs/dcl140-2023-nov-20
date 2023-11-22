/* 
 * parse.c : use whitespace to tokenise a line 
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {

  static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  int count = 0;
  char * token;
  char **newArgv;

  /* Nothing entered. */
  if (line == NULL) {
    return NULL;
  }

  /*
   * Init strtok with commandline, then get first token.
   * Return NULL if no tokens in line.
   */
  if ((token = strtok(line,delim)) == NULL) {
    return NULL;
  }

  /* Create array with room for first token. */
  newArgv = malloc(sizeof(char *));

  /* While there are more tokens... */
  while (token != NULL) {

    /* Give token its own memory, then install it. */
    newArgv[count++] = strdup(token);

    token = strtok(NULL, delim);	/* Get next token. */

    newArgv = (char **)realloc(newArgv, (count+1)*sizeof(char *)); /* Resize. */
  }

  /* Cap off the array and return it. */
  newArgv[count] = NULL;
  return newArgv;
}


/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv) {

  int i;

  for (i = 0; oldArgv[i] != NULL; i++) {
    free(oldArgv[i]); /* Free token hanging off the array. */
  }
  free(oldArgv);	/* This frees the oldArgv array itself. */
}
