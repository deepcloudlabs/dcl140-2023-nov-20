/*
 * lookup7 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Unix TCP Sockets
 * The name of the socket is passed as resource.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
  static int sockfd;
  static struct sockaddr_un server;
  static int first_time = 1;

  if (first_time) {     /* connect to socket ; resource is socket name */
    first_time = 0;

    /* Set up destination address. */
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path,resource);

    /* Allocate socket. */
    if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0) {
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

