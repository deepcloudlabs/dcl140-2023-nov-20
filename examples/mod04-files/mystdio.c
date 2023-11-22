#include <stdio.h>
#include <stdlib.h>

int main() {

  FILE *fp;
  char buf[512];
  char *filename = "foo";
  int num;

  /* opens "filename" for read & write */
  fp = fopen(filename, "r+");

  if( fp == NULL ) {
    fprintf(stderr, "Error: fopen failed on %s\n", filename);
    exit(1);
  }

  /* reads  200 bytes into buf */
  num = fread(buf, 1, 200, fp);
  fflush( fp );

  /* writes 200 bytes from buf */
  num = fwrite(buf, 1, num, fp);
  fclose(fp);
}
