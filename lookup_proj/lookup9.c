/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
  static int sockfd;
  static struct sockaddr_in server;
  struct hostent *host;
  static int first_time = 1;

  if (first_time) {  /* Set up server address & create local UDP socket */
    first_time = 0;

    /* Set up destination address. */
    server.sin_family = AF_INET;
    server.sin_port = PORT;
    if ((host = gethostbyname(resource)) == NULL) {
      return UNAVAIL;
    }
    memcpy(&server.sin_addr,host->h_addr,host->h_length);

    /* Allocate a socket. */
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
      return UNAVAIL;
    }
  }

  /* send a datagram & await reply */
  /* The first time this happens, an ephemeral port will be bound */
  if (sendto(sockfd, sought->word,strlen(sought->word) + 1, 0,
      (struct sockaddr *)&server,sizeof(server)) == -1) {
    return UNAVAIL;
  }

  if (recvfrom(sockfd, sought->text,sizeof(sought->text), 0,
      NULL,NULL) == -1) {
    return UNAVAIL;
  }

  if (strcmp(sought->text,"XXXX") != 0) {
    return FOUND;
  }

  return NOTFOUND;
}

