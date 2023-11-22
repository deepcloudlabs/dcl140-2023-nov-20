#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {

  time_t t;
  char then[30];
  char *now;

  time(&t);

  /* ctime() put the current time into static space */
  now = ctime(&t);

  /* Copy the data into a new space */
  strcpy(then, now);

  /* Let time pass for one minute */
  sleep(60);

  time(&t);

  /* ctime() puts new time into old static space */
  now = ctime(&t);

  printf("%s%s", then, now);
}
