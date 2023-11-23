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
  if(stat(argv[1],&stbuff) == -1){
      perror(argv[0]);
      exit(errno);
  }

  /* Open FIFO for reading (blocks until a client connects) */
  if(stat(argv[2],&stbuff) == -1){
      perror(argv[0]);
      exit(errno);
  }
  if(!S_ISFIFO(stbuff.st_mode)){
     fprintf(stderr,"%s is not a pipe.",argv[2]);
     exit(1); 
  }
  if ((read_fd = open(argv[2],O_RDONLY)) == -1){
      perror(argv[0]);
      exit(errno);
  }
  /* Sit in a loop. lookup word sent and send reply down the given FIFO */
  for (;;) {

    /* Read request. */
    if(read(read_fd,&cli,sizeof(Client)) == 0) {
        close(read_fd);
        if ((read_fd = open(argv[2],O_RDONLY)) == -1){
           perror(argv[0]);
           exit(errno);
        }
        continue;
    }      
    /* Get name of reply fifo and attempt to open it. */
    write_fd = open(cli.id,O_WRONLY); 
    /* lookup the word , handling the different cases appropriately */
    /* Fill in code. */
    switch(lookup(&tryit,argv[1]) ) {
      case FOUND: 
        write(write_fd,tryit.text,strlen(tryit.text)+1);
        break;
      case NOTFOUND: 
        write(write_fd,"NOT FOUND",10);
        break;
      case UNAVAIL:
        exit(2); 
    }

    /* close connection to this client (server is stateless) */
    close(write_fd);    
  }
}
