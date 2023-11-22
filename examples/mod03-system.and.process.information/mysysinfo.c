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
  printf("Loads 1,5,15: %ld,%ld,%ld\n", si.loads[0],si.loads[1],si.loads[2]);
  return 0;
}
