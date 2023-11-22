#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()   {

  int rv;

  /* Sometimes useful for troubleshooting, */
  /* E.g., do "ps" command inside program and save output */
  rv = system("ps -le | grep mysystem > /tmp/junk");

  if ( rv != 0 ) {
    fprintf(stderr, "Something went wrong!\n");
  }
}
