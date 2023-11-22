#include <string.h>
#include <stdlib.h>

int main() {

  char str[80];
  char *cp;

  /* Copies 3 bytes into str; 'a', 'b', and '\0' */
  strcpy(str, "ab");

  /* Appends "de" to str */
  strcat(str, "de");

  /* Returns the length of string "ab", which is 2 */
  strlen("ab");

  /* Allocates 3 bytes of space, and copies "ab" into it */
  cp = strdup("ab");

  /* Free the address held by cp */
  free(cp);

  /* Return the pointer to the character 'e' in the string "hello" */
  cp = strchr("hello", 'e');
}
