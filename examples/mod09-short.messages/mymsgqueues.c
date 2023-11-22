#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 80
struct msgbuf {
   long mtype;
   char mtext[1];
};

int main() {

  struct msgbuf *sendmsg;
  struct msgbuf *recvmsg;
  int msgqid;
  int rval = 0;

  sendmsg = (struct msgbuf *)malloc(sizeof(long)+BUFSIZE);
  recvmsg = (struct msgbuf *)malloc(sizeof(long)+BUFSIZE);

  if (sendmsg == NULL || recvmsg == NULL) {
    fprintf(stderr, "Out of virtual memory.\n");
    exit(1);
  }
  msgqid = msgget( IPC_PRIVATE, IPC_CREAT | 0600);

  if (msgqid == -1) {
    perror("msgget");
    exit(1);
  }
  system("ipcs");
  sendmsg->mtype = 1L;
  (void)strcpy(sendmsg->mtext, "Hello Mars.");

  if(msgsnd(msgqid,sendmsg,strlen(sendmsg->mtext)+1,0) == -1){
    perror("msgsnd");
    rval = 1;

    /* "out" is a label near the end of main */
    goto out;
  }

  sendmsg->mtype = 2L;
  (void)strcpy(sendmsg->mtext, "Hello Moon.");

  if(msgsnd(msgqid,sendmsg,strlen(sendmsg->mtext)+1,0) == -1){
    perror("msgsnd");
    rval = 1;

    /* "out" is a label near the end of main */
    goto out;
  }

  if (msgrcv(msgqid, recvmsg, BUFSIZE, 2L, 0) == -1) {
    perror("msgrcv");
    rval = 1;
    goto out;
  }
  printf("\nReceived message *%s*\n\n", recvmsg->mtext);

out:
  msgctl(msgqid, IPC_RMID, (struct msqid_ds *)NULL);
  system("ipcs");
  exit(rval);
}
