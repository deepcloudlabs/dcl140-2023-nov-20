/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "dict.h"

int main(int argc, char **argv) {
    struct sockaddr_un server;
    int sd,cd,n;
    Dictrec tryit;

    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }

    /* Setup socket for listening */
    if ((sd = socket(AF_UNIX,SOCK_STREAM,0)) == -1) {
      DIE("Failed to create socket.");
    }
    
    /* Initialize address. */
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path,argv[2]);

    /* Name and activate the socket. */
    unlink(server.sun_path);
    if (bind(sd,(struct sockaddr *)&server,sizeof(server)) == -1) {
      DIE("Failed to make socket");
    }

    if (listen(sd,128) == -1) {
      DIE ("Failed to listen on socket");
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

	    /* Lookup request. */
            switch(lookup(&tryit,argv[1]) ) {

	      /* Write response back to client. */
              case FOUND: 
                if (write(cd,tryit.text,strlen(tryit.text) + 1) == -1) {
		  perror ("Socket write of response");
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

	  /* Terminate child process.  It is done. */
          exit(0);

	/* Parent continues here. */
        default :
          close(cd);
          break;
      } /* end fork switch */
    } /* end forever loop */
    return 0;
} /* end main */

