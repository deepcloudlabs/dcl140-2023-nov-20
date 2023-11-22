#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void cleanup() {

  char *message = "cleanup invoked\n";

  write(STDOUT_FILENO, message, (size_t) strlen(message));
}

int main() {

  /* Register cleanup() as atexit function */
  atexit(cleanup);  

  /* Other things in program done here */

  exit(0);
}
