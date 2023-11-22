#include <sys/types.h>
#include <time.h>
#include <stdio.h>

int main() {

  /* Declare an object and pass its address */
  time_t t;

  time(&t);
  printf("Machine time in seconds = %ld\n", t);
}
