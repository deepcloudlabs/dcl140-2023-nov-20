#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

typedef void (*sighandler_t)(int);

void handler(int signo) {

  write(1, "Hi, I'm the signal handler!\n", 29);

  /* If absent, execution resumes where interrupted */
  exit(0);  
}

int main() {
  sighandler_t ohand;

  /* Install handler() */
  ohand = sigset(SIGINT, handler);

  if (ohand == SIG_ERR) {
    perror("sigset");
  }

  /* do some stuff */

  /* Reinstall ohand */
  (void)sigset(SIGINT, ohand);
}
