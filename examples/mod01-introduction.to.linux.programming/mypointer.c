#include <stdio.h>

int main( int argc, char **argv) {

  char **tmp;

  for( tmp = argv; *tmp != NULL; tmp++ ) {
    printf("Argument %ld = %s %x\n", tmp - argv, *tmp,tmp);
  }
  return 0;
}
