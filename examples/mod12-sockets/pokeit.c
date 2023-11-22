/* For testing pollProg.c */

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

int udpsd1;
int udpsd2;
struct sockaddr_in destination1;
struct sockaddr_in destination2;

void handler(int signo) {
  static int counter;
  if (!(counter % 2)) {
    printf ("UDP1 sendto\n");
    if (sendto(udpsd1, "Hello", 6, 0,
	(struct sockaddr *)&destination1, sizeof(destination1))) {
      perror ("sendto");
    }
  }
  if (!(counter % 3)) {
    printf ("UDP2 sendto\n");
    if (sendto(udpsd2, "Goodbye", 8, 0,
	(struct sockaddr *)&destination2, sizeof(destination2))) {
      perror ("sendto");
    }
  }
  counter++;
}

int main() {
  struct sockaddr_in udpsource1;
  struct sockaddr_in udpsource2;
  struct itimerval period;
  struct hostent * he;
  struct utsname uts;
  sigset_t mask;
  struct sigaction alarmAction;

  udpsd1 = socket(AF_INET, SOCK_DGRAM, 0);
  udpsd2 = socket(AF_INET, SOCK_DGRAM, 0);

  he = (struct hostent *)gethostbyname("localhost");

  memset(&destination1, '\0', sizeof(destination1));
  destination1.sin_family = AF_INET;
  destination1.sin_port = htons(7000);
  memcpy((char *)&destination1.sin_addr, he->h_addr_list[0], he->h_length); 

  memset(&destination2, '\0', sizeof(destination2));
  destination2.sin_family = AF_INET;
  destination2.sin_port = htons(7001);
  memcpy((char *)&destination2.sin_addr, he->h_addr_list[0], he->h_length); 

  if (uname (&uts) == -1) {
    perror ("uname");
    exit (errno);
  }

  he = (struct hostent *)gethostbyname(uts.nodename);

  memset(&udpsource1, '\0', sizeof(udpsource1));
  udpsource1.sin_family = AF_INET;
  udpsource1.sin_port = htons(6998);
  memcpy((char *)&udpsource1.sin_addr, he->h_addr_list[0], he->h_length); 

  memset(&udpsource2, '\0', sizeof(udpsource2));
  udpsource2.sin_family = AF_INET;
  udpsource2.sin_port = htons(6999);
  memcpy((char *)&udpsource2.sin_addr, he->h_addr_list[0], he->h_length); 

  if (bind(udpsd1, (struct sockaddr *)&udpsource1, sizeof(udpsource1))) {
    perror ("bind");
    exit (errno);
  }

  if (bind(udpsd2, (struct sockaddr *)&udpsource2, sizeof(udpsource2))) {
    perror ("bind");
    exit (errno);
  }

  alarmAction.sa_handler = handler;
  alarmAction.sa_flags = SA_RESTART;
  sigaction(SIGALRM, &alarmAction, (struct sigaction *)NULL);

  sigfillset(&mask);
  sigdelset(&mask, SIGALRM);
  sigdelset(&mask, SIGINT);

  period.it_interval.tv_sec = 1;
  period.it_interval.tv_usec = 0;
  period.it_value.tv_sec = 1;
  period.it_value.tv_usec = 0;

  if (setitimer(ITIMER_REAL, &period, (struct itimerval *)NULL)) {
    perror ("getitimer");
    exit (errno);
  }

  while (1) {
    sigsuspend(&mask);
  }
}
