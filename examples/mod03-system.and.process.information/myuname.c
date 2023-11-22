#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  struct utsname uts;

  if( uname(&uts) == -1 ) {
    perror("myuname.c:main:uname");
    exit(1);
  }

  printf("operating system: %s\n", uts.sysname);
  printf("hostname: %s\n", uts.nodename);
  printf("release: %s\n", uts.release);
  printf("version: %s\n", uts.version);
  printf("machine: %s\n", uts.machine);
}
