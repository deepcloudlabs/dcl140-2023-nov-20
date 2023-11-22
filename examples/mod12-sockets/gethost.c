/* gethost.c */ 

#include <sys/types.h>
#include <sys/socket.h>  
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  struct hostent *he;
  char    **tmp;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s hostname\n", argv[0]); 
    exit(1);
  }
  he = gethostbyname(argv[1]);

  if (he == NULL) {
    fprintf(stderr, "No such host.\n");
    exit(1);
  }
  printf("Official name of %s is %s.\n", 
          argv[1], he->h_name);

  for (tmp=he->h_aliases; *tmp != NULL; tmp++) {
    printf("\tAlias: %s\n", *tmp);
  }

  tmp = he->h_addr_list;
  printf("IP address: %d.%d.%d.%d\n",
            (unsigned char)(*tmp)[0], 
            (unsigned char)(*tmp)[1], 
            (unsigned char)(*tmp)[2], 
            (unsigned char)(*tmp)[3]);
  return 0;
}
