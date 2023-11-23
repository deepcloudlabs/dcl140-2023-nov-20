/*
 * udp_server : listen on a UDP socket ;reply immediately
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

#include "dict.h"

int main(int argc, char **argv) {
  static struct sockaddr_in server,client;
  int sockfd,siz;
  Dictrec dr, *tryit = &dr;

  if (argc != 2) {
    fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
    exit(errno);
  }

  /* Create a UDP socket & bind to well-known port */
  if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
    DIE("Failed to create socket");
  }

  /* Initialize address. */
  server.sin_family = AF_INET;
  server.sin_port = PORT;

  /* Name and activate the socket. */
  if (bind(sockfd,(struct sockaddr *)&server,sizeof(server)) == -1) {
    DIE("failed to make sock");
  }

  for (;;) { /* await client packet; respond immediately */

    siz = sizeof(client); /* siz must be non-zero */

    /* Wait for a request. */
    if (recvfrom(sockfd, tryit->word,sizeof(tryit->word), 0,
        (struct sockaddr *)&client,&siz) == -1) {
      perror ("Error receiving request.");
      continue;
    }

    /* Lookup request and respond to user. */
    switch(lookup(tryit,argv[1]) ) {
      case FOUND: 
        if (sendto(sockfd, tryit->text,strlen(tryit->text) + 1, 0,
            (struct sockaddr *)&client,siz) == -1) {
	  perror ("Error sending response");
	}
        break;
      case NOTFOUND : 
        if (sendto(sockfd, "XXXX",5, 0,(struct sockaddr *)&client,siz) == -1) {
	  perror ("Error sending response");
	}
        break;
      case UNAVAIL:
	DIE(argv[1]);
    } /* end lookup switch */
  } /* end forever loop */
  return 0;
} /* end main */

