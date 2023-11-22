#include <sys/types.h>
#include <time.h>
#include <stdio.h>

#define BUFSIZE 9

int main() {

  time_t t;
  struct tm *tmptr;
  char buf[BUFSIZE];

  time(&t);
  tmptr = localtime(&t);
  strftime(buf, BUFSIZE, "%m-%d-%y", tmptr);
  printf("%s\n", buf);
}
