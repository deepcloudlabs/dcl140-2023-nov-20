/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "dict.h"

int main(int argc, char **argv) {
  static struct sockaddr_in server;
  int sd,cd,n;
  Dictrec tryit;

  if (argc != 2) {
    fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
    exit(1);
  }

  /* setup the socket for listening */
  if ((sd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
    DIE("Failed to create socket.");
  }

  /* Initialize address. */
  server.sin_family = AF_INET;
  server.sin_port = PORT;

  /* Name and activate the socket. */
  if (bind(sd,(struct sockaddr *)&server,sizeof(server)) == -1) {
    DIE("failed to make sock");
  }

  if (listen(sd,128) == -1) {
    DIE("Failed to listen on socket");
  }

  /* main loop : accept connection; fork a child to have dialogue */
  for (;;) {

    /* Wait for a connection. */
    cd = accept(sd,NULL,NULL);

    /* Handle new client in a subprocess. */
    switch (fork()) {

      case -1 : 
        DIE("fork");

      case 0 :

        close (sd);	/* Rendezvous socket is for parent only. */

	/* Get next request. */
        while (read(cd,tryit.word,WORD)) {

          /* Lookup the word , handling the different cases appropriately */
          switch(lookup(&tryit,argv[1]) ) {

	    /* Write response back to the client. */
            case FOUND: 
              if (write(cd,tryit.text,strlen(tryit.text) + 1) == -1) {
		perror ("Socket write or response");
	      }
              break;
            case NOTFOUND: 
              if (write(cd,"XXXX",5) == -1) {
		perror ("Socket write of response");
	      }
              break;
            case UNAVAIL:
	      DIE(argv[1]);
          } /* end lookup switch */
        } /* end of client dialog */
        exit(0); /* child does not want to be a parent */

      default :
        close(cd);
        break;
      } /* end fork switch */
    } /* end forever loop */
    return 0; 
} /* end main */

