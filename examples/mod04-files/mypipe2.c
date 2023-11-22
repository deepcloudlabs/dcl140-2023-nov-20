#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

  int p[2];
  pid_t pid;

  if ( pipe(p) == -1 ) {
    perror("pipe");
    exit(1);
  }

  switch( pid = fork() ) {

  case -1:
    perror("fork");
    exit(1);
    break;

  /* child */
  case 0:
    close(p[1]);

    if (p[0] != 0) {
      dup2(p[0], 0);
      close(p[0]);
    }

    execlp("mailx","mailx","-s","Error","deac",(char *)NULL);
    perror("execlp");
    exit(1);
    break;

  /* parent */
  default:
    break;
  }

  close(p[0]);

  /* if an error occurs in the program, then do: */
  write(p[1], "An error occurred.\n", 18);
  close(p[1]);

  if (waitpid(pid, (int *)NULL, 0) == -1) {
    perror("waitpid");
    exit(1);
  }
}
