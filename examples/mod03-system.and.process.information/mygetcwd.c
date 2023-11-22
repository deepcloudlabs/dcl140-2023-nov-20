#include <sys/param.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  char  *dir;
  long  pathmaxlen = pathconf(".", _PC_PATH_MAX);  

  dir = getcwd((char *)NULL, pathmaxlen + 1);

  if(dir == NULL) { 
    perror("getcwd"); 
    exit(1); 
  }

  printf("CWD: %s\n", dir);
  free(dir);

  if(chdir("/tmp") == -1) {
    perror("chdir");
  }

  pathmaxlen = pathconf(".", _PC_PATH_MAX);  
  dir = getcwd((char *)NULL, pathmaxlen + 1);

  if (dir == NULL) {
    perror("getcwd");
  }

  printf("CWD: %s\n", dir);

  free(dir);
}
