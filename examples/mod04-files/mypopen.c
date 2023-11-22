#include <stdio.h>

int main() {

  FILE *fp;
  int m;

  /* Write your screen output through "more" */
  fp = popen("more", "w");

  if (fp == NULL) {
    fprintf(stderr, "popen failed.\n");
    return(1);
  }

  for( m = 1; m <= 100; m++ ) {
    fprintf(fp, "Lots and lots of stuff.\n");
  }

  pclose(fp);
  return(0);

  /* Note: Could have used: fwrite(buf, size, n, fp) */
}
