#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

/*********************
install_disp  -  An encapsulation function which installs 
a signal handling disposition using the same interface as 
signal and sigset BUT has BSD semantics and is POSIX 
compliant. Works on both SunOS 4.x and 5.x as noted.
*********************/
void (*install_disp(int signo, void (*disp)(int)) ) (int) {

  /* new action to be installed */
  struct sigaction act;

  /* current action to be replaced */
  struct sigaction oldact;

  act.sa_handler = disp;
  sigemptyset(&act.sa_mask);

  /* For 4.x */
  act.sa_flags = 0;

  /* OK for 5.x */
  act.sa_flags = SA_RESTART;

  if( sigaction( signo, &act, &oldact ) == -1 ) {
    return( SIG_ERR );
  }
  return( oldact.sa_handler);
}

void handler(int signo) {

   char *cp = "Hi, I'm in the handler\n";
   write(1,cp,strlen(cp));
}

int main() {

   printf("\nPress ^\\ or ^Z to call handler,^C to terminate\n\n");
   install_disp(SIGQUIT, handler); 
   install_disp(SIGTSTP, handler); 
   while(1)
      ;    
}
