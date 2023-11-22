#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
  printf("My uid is %d\n", getuid() );
}
