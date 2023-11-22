#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define NAMESIZE 20

int main() {

  char filename[NAMESIZE];
  int fd;

  strcpy(filename,"abcdefg");

  if ((fd = open(filename, O_RDWR)) == -1) {
    fprintf (stderr, "Error opening %s: %s\n",
        filename, strerror(errno));
    exit (errno);
  }
}
