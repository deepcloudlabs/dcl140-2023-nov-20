#include <sys/types.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

  int fd_in;
  int fd_out;
  caddr_t addr_in;
  caddr_t addr_out;
  struct stat statbuf;

  if( argc != 3 ) {
    fprintf(stderr,"Usage: mycp input output\n");
    exit(1);
  }

  if( stat(argv[1], &statbuf) == -1 ) { 
    perror("stat"); 
    exit(1); 
  }

  fd_in = open( argv[1], O_RDONLY );
  if (fd_in == -1) { 
    perror("open"); 
    exit(1); 
  }

  addr_in = mmap((caddr_t)NULL, statbuf.st_size, 
      PROT_READ, MAP_SHARED, fd_in, (off_t)0);
  if (addr_in == MAP_FAILED) { 
    perror("mmap");
    exit(1); 
  }

  /* no longer need fd */
  close(fd_in);
  fd_out = open( argv[2], O_RDWR | O_CREAT | O_TRUNC , statbuf.st_mode);

  if (fd_out == -1) { 
    perror("open"); 
    exit(1); 
  }
  ftruncate(fd_out,statbuf.st_size);

  addr_out = mmap((caddr_t)NULL, statbuf.st_size, 
      PROT_WRITE, MAP_SHARED, fd_out, (off_t)0);
  if (addr_out == MAP_FAILED) { 
    perror("mmap");
    exit(1); 
  }
  memcpy(addr_out , addr_in, statbuf.st_size);
  //munmap(addr,statbuf.st_size);
  return(0);
}
