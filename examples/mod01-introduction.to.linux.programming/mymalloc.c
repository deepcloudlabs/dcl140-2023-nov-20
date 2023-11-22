#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  struct timeval *tvp;
  char *date;

  tvp = (struct timeval *)malloc(sizeof(struct timeval));
  if (tvp == NULL) {
    fprintf(stderr, "Out of virtual memory.\n");
    exit(1);
  }

  if (gettimeofday(tvp, NULL) == -1) {
    perror("gettimeofday failed");
    exit(1);
  }

  date = strdup(ctime(&tvp->tv_sec));

  if (date == NULL) {
    fprintf(stderr, "Out of virtual memory.\n");
    exit(1);
  }

  printf("%s", date);

  free(tvp);
  free(date);

  return 0;
}
