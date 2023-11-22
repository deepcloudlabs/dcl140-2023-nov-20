#include <string.h>

int main() {

  /* Returns a pointer to the first occurence of any 
     character in the string "def" that is found in the 
     string "hello"; in this case the character 'e' */
  strpbrk("hello", "def");

  /* Returns a pointer to the first occurence of "cd" found
     in string "abcdef". Returns NULL if none is found */
  strstr("abcdef", "cd");

  /* Returns the length of the initial segment of a given 
     string. In this case, 3 is returned for the length
     of "123", common in both strings. */
  strspn("123def4", "0123456789");

}
