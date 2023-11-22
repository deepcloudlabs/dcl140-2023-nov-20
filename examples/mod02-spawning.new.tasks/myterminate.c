#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

  pid_t  pid;
  int    status;

  /* fork() a child */
  switch(pid = fork()) {   

  case -1:
    perror("fork");
    exit(1);

  /* in child */
  case 0:
    execlp("ls", "ls", "-F", (char *)NULL);
    perror("execlp");
    exit(1);

  /* parent */
  default:
    break;
  }

  if (waitpid(pid, &status, 0) == -1) {
    perror("waitpid");
    exit(1);
  }

  /* See wstat(5) for macros used with status from wait(2) */
  if (WIFSIGNALED(status)) {
    printf("ls terminated by signal %d.\n", WTERMSIG(status));
  } else if (WIFEXITED(status)) {
    printf("ls exited with status %d.\n", WEXITSTATUS(status));
  } else if (WIFSTOPPED(status)) {
    printf("ls stopped by signal %d.\n", WSTOPSIG(status));
  }
  return 0;
}
