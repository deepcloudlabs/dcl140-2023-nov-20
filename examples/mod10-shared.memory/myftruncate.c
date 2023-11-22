#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  int fd;
  int pagesize = sysconf(_SC_PAGESIZE);
  caddr_t addr;

  printf("Page size is %d bytes.\n",pagesize); 

  fd = open("mapfile", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (fd == -1) {
    perror("open");
    exit(1);
  }

  if (ftruncate(fd, (off_t)(6 * pagesize)) == -1) {
    perror("ftruncate");
    exit(1);
  }

  system("ls -l mapfile");

  addr = mmap((caddr_t)NULL, 6*pagesize, PROT_READ |
      PROT_WRITE, MAP_SHARED, fd, (off_t)0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  /* no longer need fd */
  close(fd);

  /* Write into file! */
  (void)strcpy(addr, "Test string.\n");

  /* Display 1 line of mapfile */
  system("head -1 mapfile");
  return(0);
}
