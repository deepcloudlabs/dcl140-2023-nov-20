#include <sys/time.h>
#include <stdio.h>

int main() {

  struct timeval tv;

  gettimeofday(&tv, NULL);
  printf("Time: seconds = %ld microseconds = %ld\n", tv.tv_sec, tv.tv_usec);
}
