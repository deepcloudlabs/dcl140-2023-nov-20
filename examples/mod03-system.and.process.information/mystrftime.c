#include <sys/types.h>
#include <time.h>
#include <stdio.h>

int main() {

  time_t t;
  struct tm *tmptr;
  char string[30];
  time(&t);
  tmptr = localtime(&t);
  strftime(string, sizeof(string), "It is now %I:%M %p", tmptr);
  printf("%s \n", string);
}
