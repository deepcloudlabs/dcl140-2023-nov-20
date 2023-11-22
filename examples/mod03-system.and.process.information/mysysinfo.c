#include <sys/sysinfo.h>
#include <stdio.h>

int main() {
  struct sysinfo si;
  int num;

  num = sysinfo(&si);
  if (num == -1) {
    perror("sysinfo");
    return 1;
  }

  printf("Total memory: %ld\n", si.totalram);
  printf("Free memory: %ld\n", si.freeram);
  return 0;
}
