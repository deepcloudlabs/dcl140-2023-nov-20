#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()   {

  mode_t  oldmask;
  int     fd; 

  /* allow group write permission temporarily */
  oldmask = umask(002);
  fd = open("testfile1", O_WRONLY | O_CREAT, 0666);
  umask(oldmask);
  fd = open("testfile2", O_WRONLY | O_CREAT, 0666);
}
