/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
  Dictrec dr;
  static FILE * in;
  static int first_time = 1;

  if (first_time) { /* open up the file */
    first_time = 0;
    if ((in = fopen(resource,"r")) == NULL)
      return UNAVAIL;
  }

  /* read from top of file, looking for match */
  rewind(in);
  while(fread(&dr,sizeof(dr),1,in) == 1)
    if (strcmp(sought->word,dr.word) == 0) {
      strcpy(sought->text,dr.text);
      return FOUND;
    }

  return NOTFOUND;
}

