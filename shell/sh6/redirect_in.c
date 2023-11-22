#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shell.h"

/*
 * Look for ">" in myArgv, then redirect output to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_in(char ** myArgv) {
  int i;
  int fd;

  /* search forward for > */
  for(i=0;myArgv[i] != NULL;i++){
     if (!strcmp(myArgv[i],"<")){
       break; 
    }
  }
  if (myArgv[i]) {	/* found ">" in vector. */
    /* Open file. */
    if (!myArgv[i+1]){
       return -1;
    }

    /* Redirect to use it for output. */
    if ((fd = open(myArgv[i+1],O_RDONLY)) == -1){
        perror("redirecting");
        return -1;
    }
    dup2(fd,STDIN_FILENO);
    /* Cleanup / close unneeded file descriptors. */
    close(fd);

    /* Remove the > and the filename from myArgv. */
    for(i=i+2;myArgv[i] != NULL;i++){
        myArgv[i-1] = myArgv[i];
    }
    myArgv[i-2] = NULL;
  }
  return 0;
}
