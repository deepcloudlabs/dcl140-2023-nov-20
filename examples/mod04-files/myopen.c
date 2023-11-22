#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

  int  fd, outfd;

  fd = open("/etc/motd", O_RDONLY);

  if (fd == -1) {
    perror("open");
    exit(1);
  }

  outfd = open("output", 
      O_WRONLY | O_CREAT | O_TRUNC, 0777);

  if (outfd == -1) {
    perror("open"); 
    exit(1);
  }

  /* Use the file descriptors here */
  close(fd);
  close(outfd);
}
