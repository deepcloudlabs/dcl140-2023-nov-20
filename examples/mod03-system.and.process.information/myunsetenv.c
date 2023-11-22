#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Function to remove an environment variable */
void myunsetenv( char *var) {

  extern char **environ;
  char **env;
  int  len;

  env = environ;
  len = strlen(var);
  while (*env) {
    if((strncmp(var, *env, len) == 0) && ((*env)[len] == '=')) {
      break; /* match */
    }
    env++;
  }

  /**********************
  if (*env) is non-NULL, then we had a match, and will
  enter the loop. If it is NULL, no match, and we will
  not enter the loop. The loop now moves all entries
  up one, to delete the unset variable.
  **********************/
  while (*env) {
    *env = *(env + 1);
    env++;
  }
  return;
}

/* Function to display an environment variable */
int checkvar(char *var) {

  char *getenv();
  char *value;

  value = getenv(var);
  if (value == NULL)
    printf("%s not defined\n", var);
  else
    printf("%s = %s\n", var, value);
}

int main() {

  myunsetenv("MYVAR0");
  putenv("MYVAR=somevalue");
  checkvar("MYVAR");
  putenv("MYVAR2=anothervalue");
  checkvar("MYVAR2");
  myunsetenv("MYVAR");
  checkvar("MYVAR");
  checkvar("MYVAR2");
}
