/*
 * lookup3 : This file does no looking up locally, but instead asks
 * a server for the answer. Communication is by named pipes.
 * The server sits listening on a "well-known" named pipe
 * (this is what is passed to lookup3 as resource)
 * The Client sets up a FIFO for reply, and bundles the FIFO
 * name with the word to be looked up. (See Client in dict.h)
 * Care must be taken to avoid deadly embrace (client & server
 * both waiting for something which can never happen)
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dict.h"

static Client me;

void cleanup(void) {
  /* Delete named pipe from system. */
  /* Fill in code. */
}

int lookup(Dictrec * sought, const char * resource) {
  static int write_fd;
  int read_fd;
  static int first_time = 1;

  if (first_time) {
    first_time = 0;

    /* Open existing named pipe for client->server communication. */
    /* Fill in code. */

    /* Create temporary named pipe for server->client communication. */
    umask(0);
    mkstemp(me.id);
    /* Fill in code. */

    /* Register cleanup handler. */
    /* Fill in code. */
  }

  /* Send server the word to be found ; await reply */
  strcpy(me.word,sought->word);
  /* Fill in code. */
  if (1 /* change this */) {
    return UNAVAIL;
  }

  /* Open the temporary named pipe to get the answer from it. */
  /* Fill in code. */

  /* Get the answer. */
  /* Fill in code. */

  /* Close the temporary named pipe as done. */
  /* Fill in code. */

  /* If word returned is not XXXX it was found. */
  if (strcmp(sought->text,"XXXX") != 0)
    return FOUND;

  return NOTFOUND;
}
