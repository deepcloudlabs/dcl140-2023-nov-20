#include <sys/types.h>
#include <time.h>
#include <stdio.h>

int main() {
  
  time_t t1;
  time_t t2;
  struct tm *tmp1;
  struct tm *tmp2;
  char timestr[40];

  time(&t1);
  printf("Today : %s", ctime(&t1));

  /* Yesterday: 24hrs*60min/hr*60sec/min */
  t2 = t1 - (24*60*60);
  printf("Yesterday: %s", ctime(&t2));

  /* A week ago */
  t2 = t1 - 7*(24*60*60);
  printf("Last Week: %s", ctime(&t2));

  /* Two weeks from now */
  t2 = t1 + 2*7*(24*60*60);
  printf("Two Weeks: %s", ctime(&t2));

  tmp2 = localtime(&t2);
  strftime(timestr, 40, "%D, Julian Date: %j ", tmp2);
  puts( timestr );
}
