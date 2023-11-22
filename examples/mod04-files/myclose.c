#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {

  int fd;
  char str[]="Howdy, Folks\n";
    
  fd = open("testfile1", O_WRONLY | O_CREAT, 0777);
  write(fd, str, strlen(str) );
    
  close(fd);
  return(0); 
}
