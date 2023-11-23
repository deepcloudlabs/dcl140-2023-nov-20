/*
 * fifo_server : listen on a named pipe; do lookup ; reply
 *               down another named pipe, the name of which
 *               will be sent by the client (in cli.id)
 *               argv[1] is the name of the local fil
 *               argv[2] is the name of the "well-known" FIFO
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "dict.h"

int main(int argc, char **argv) {
  struct stat stbuff;
  int read_fd,write_fd;
  Client cli;
  Dictrec tryit;

  if (argc != 3) {
    fprintf(stderr,"Usage : %s <dictionary source> "
        "<resource / FIFO>\n",argv[0]);
    exit(errno);
  }

  /* Check for existence of dictionary and FIFO (both must exist) */
  /* Fill in code. */

  /* Open FIFO for reading (blocks until a client connects) */
  /* Fill in code. */

  /* Sit in a loop. lookup word sent and send reply down the given FIFO */
  for (;;) {

    /* Read request. */
    /* Fill in code. */

    /* Get name of reply fifo and attempt to open it. */
    /* Fill in code. */

    /* lookup the word , handling the different cases appropriately */
    /* Fill in code. */
    switch(lookup(&tryit,argv[1]) ) {
      case FOUND: 
        /* Fill in code. */
        break;
      case NOTFOUND: 
        /* Fill in code. */
        break;
      case UNAVAIL:
        break;
        /* Fill in code. */
    }

    /* close connection to this client (server is stateless) */
    /* Fill in code. */
  }
}
