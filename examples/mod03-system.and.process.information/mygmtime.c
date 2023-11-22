#include <sys/types.h>
#include <time.h>
#include <stdio.h>

int main() {
  time_t t;
  struct tm *tmptr;

  time(&t);
  tmptr = localtime(&t);

  printf("The year is %d\n", tmptr->tm_year);
  printf("The day of the year is %d\n", tmptr->tm_yday);
  printf("Hour %d\n", tmptr->tm_hour);
}
