/*
 * convert.c : take a file in the form 
 *  word1
 *  multiline definition of word1
 *  stretching over several lines, 
 * followed by a blank line
 * word2....etc
 * convert into a file of fixed-length records
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "dict.h"
#define BIGLINE 512

int main(int argc, char **argv) {
  FILE *in = stdin, * out = stdout;        /* defaults */
  char line[BIGLINE];
  char * newlineChar;
  int eof = 0,off,newoff;
  static Dictrec dr,blank;

  /* If args are supplied, argv[1] is for input, argv[2] for output */

  if (argc > 1) {
    fclose(in);
    if ((in =fopen(argv[1],"r")) == NULL)
      DIE(argv[1]);
  }
  if (argc > 2) {
    fclose(out);
    if ((out =fopen(argv[2],"w")) == NULL)
      DIE(argv[2]);
  }
    
  /* Main reading loop : read word first, then definition into dr */

  /* Loop through the whole file. */
  while (!eof) {

    /* Create and fill in a new blank record.
     * First get a word and put it in the word field, then get the definition
     * and put it in the text field at the right offset.  Pad the unused chars
     * in both fields with nulls.
     */

    /* Read word and put in record.  Truncate at the end of the "word" field. */
    /* Fill in code. */

    /* Read definition, line by line, and put in record. */
    /* Fill in code. */

    /* Write record out to file. */
    /* Fill in code. */
  }

fclose(in);
fclose(out);
}
