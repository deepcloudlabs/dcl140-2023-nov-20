#include <stdlib.h>
#include <stdio.h>

int main() {

  char *value;

  value = getenv("HOME");
  if( value == NULL ) {
    printf("HOME is not defined\n");
  } else if( *value == '\0') {
    printf("HOME defined but has no value\n");
  } else {
    printf("HOME = %s\n", value);
  }
}
