#include <string.h>

int main() {

  char *cp;
  char buf[]="Hello World";

  /* Make cp point to Hello\0 */
  cp = strtok(buf, " ");

  /* Now make cp point to World\0 */
  cp = strtok(NULL, " ");

  /* Now make cp point NULL */
  cp = strtok(NULL, " ");
}
