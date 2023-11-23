/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
  static int sockfd;
  static struct sockaddr_in server;
  struct hostent *host;
  static int first_time = 1;

  if (first_time) {        /* connect to socket ; resource is server name */
    first_time = 0;

    /* Set up destination address. */
    server.sin_family = AF_INET;
    server.sin_port = PORT;
    if ((host = gethostbyname(resource)) == NULL) {
      return UNAVAIL;
    }
    memcpy(&server.sin_addr,host->h_addr,host->h_length);

    /* Allocate socket. */
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
      return UNAVAIL;
    }

    /* Connect to the server. */
    if (connect(sockfd,(struct sockaddr *)&server,sizeof(server)) == -1)
      return UNAVAIL;
  }

  /* write query on socket ; await reply */
  if (write(sockfd,sought->word,strlen(sought->word) + 1) == -1) {
    return UNAVAIL;
  }

  if (read(sockfd,sought->text,sizeof(sought->text)) == -1) {
    return UNAVAIL;
  }

  if (strcmp(sought->text,"XXXX") != 0) {
    return FOUND;
  }

  return NOTFOUND;
}

