#include <unistd.h>
#include <stdio.h>

int main() {
  printf("Maximum filename length: %ld\n", pathconf(".", _PC_NAME_MAX));
  printf("Maximum path length: %ld\n", pathconf("/", _PC_PATH_MAX));
  printf("Pipe buffer size: %ld\n", pathconf("/var/spool/cron/FIFO", _PC_PIPE_BUF));
}
