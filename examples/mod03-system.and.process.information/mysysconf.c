#include <sys/unistd.h>
#include <stdio.h>

int main() {

  printf("Number of processors: %ld\n", sysconf(_SC_NPROCESSORS_CONF));
  printf("Memory page size: %ld\n", sysconf(_SC_PAGESIZE));
  printf("Clock ticks/second: %ld\n", sysconf(_SC_CLK_TCK));
  printf("Number of files that can be opened: %ld\n", sysconf(_SC_OPEN_MAX));
}
