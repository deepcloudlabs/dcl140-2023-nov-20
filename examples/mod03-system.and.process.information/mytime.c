#include <sys/types.h>
#include <time.h>
#include <stdio.h>

int main() {

  time_t t;

  time(&t);
  printf("Time in seconds = %ld\n", t);
}
