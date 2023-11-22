#include <stdio.h>
#include <stdlib.h>

int main() {

  if( freopen("/tmp/foo3", "w", stdout) != stdout ) {  
    fprintf(stderr,"ERROR: reopen\n"); 
    exit(1); 
  }

  /* stdout is now "/tmp/foo3" */
  printf("Hi Folks!\n");
}
