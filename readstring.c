#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readstring(FILE *infile) {
   /* makes the string lower case too */
   static char *buffer = NULL;
   char *final;
   int c; 
   int len;
   int size = 50;

   buffer = (char*)malloc(size * sizeof(char));

   for (c = getc(infile); (c != EOF) && isalpha(c); c = getc(infile))
      /* goes until we reach our first character */

   for (len = 0; isalpha(c) && (c != EOF); c = getc(infile)) {
      if (size < (len + 5)) {  /* if our buffer is too small, increase size */
         size += 50;
         buffer = (char*)realloc(buffer, size * sizeof(char));
      }
      buffer[len++] = tolower(c); /* append character to buffer */
   }
   buffer[len] = '\0'; /* append null character to signify end of word */
  
   if (len > 0)  /* if our string has anything in it, we want to return that. */
      final = buffer;
   else  /* otherwise, just return nothing */
      final = NULL;
   return final;
}
