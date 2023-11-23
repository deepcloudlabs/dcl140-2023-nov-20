#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dict.h"

static Client me;

void cleanup(void) {
  unlink(me.id);	/* Delete named pipe from system. */
}

int lookup(Dictrec * sought, const char * resource) {
  static int write_fd;
  int read_fd;
  static int first_time = 1;

  if (first_time) {
    first_time = 0;

    /* Open existing named pipe for client->server communication. */
    if ((write_fd = open(resource,O_WRONLY)) == -1) {
      return UNAVAIL;
    }

    /* Create temporary named pipe for server->client communication. */
    umask(0);
    mkdtemp(me.id);
    if (mkfifo(me.id,0666) == -1) {
      return UNAVAIL;
    }

    /* Register cleanup handler. */
    atexit(cleanup);
  }

  /* Send server the word to be found ; await reply */
  strcpy(me.word,sought->word);

  if (write(write_fd,&me,sizeof(me)) == -1) {
    return UNAVAIL;
  }

  /* Open the temporary named pipe to get the answer from it. */
  if ((read_fd = open(me.id,O_RDONLY)) == -1) {
    return UNAVAIL;
  }

  /* Get the answer. */
  if (read(read_fd,sought->text,sizeof(sought->text)) == -1) {
    return UNAVAIL;
  }

  /* Close the temporary named pipe as done. */
  close(read_fd);

  /* If word returned is not XXXX it was found. */
  if (strcmp(sought->text,"XXXX") != 0)
    return FOUND;

  return NOTFOUND;
}

