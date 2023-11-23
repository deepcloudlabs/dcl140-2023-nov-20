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
    dr = blank;   /* copy blank record */

    /* Read word.  Truncate at the end of the "word" field. */
    if (fgets(dr.word,BIGLINE,in) == NULL ) /* try to read word */
      break;
    dr.word[sizeof(dr.word) - 1] = '\0'; /* Null terminate the field. */
    newlineChar = strchr(dr.word, '\n');/* Null terminate at \n. */
    if (newlineChar) {
      *newlineChar = '\0';
    }
    fprintf(stderr,"\"%s\"\n",dr.word);
    /* Read definition. !Append lines directly into dr until run out of space.*/
    newoff = off = 0;
    while ((sizeof(dr.text)-off) > 0) {
      eof = (fgets(dr.text + off,sizeof(dr.text)-off,in) == NULL);
      newoff = strlen(dr.text);
       
      /* Stop appendingif EOF, blank line, or last char is not \n (line was
      truncated as not enough room in dr.text field.) */
      if ( eof || ((newoff - off) < 2) || (dr.text[newoff-1] != '\n')) {
        break;

      /* Keep appending, but start overwriting last char (assumed a \n). */
      } else {
        off = newoff-1;
      }
    }

    /* Not blank line nor EOF, so flush the rest of this definition. */
    if (!( eof || ((newoff - off) < 2))) {
      do {
        eof = fgets(line,BIGLINE,in) == NULL;
      } while (strlen(line) >= 2);
    }

    fwrite(&dr,sizeof(dr),1,out); /* write out complete record */
  }


  fclose(in);
  fclose(out);
  return 0;
}
