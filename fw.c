#include <stdlib.h>
#include <string.h>

#include "readstring.h"
#include "hashtab.h"

int main(int argc, char *argv[]) {
   FILE *infile;
   int num_words = 10; /* default word count */
   int num_of_files = 1;
   char *current_word;
   struct nlist **res;

   if (argc > 1) {
      if (!(strcmp(argv[1], "-n"))) {
         if (argc > 2) {
            num_words = atoi(argv[2]);
            if (num_words == 0) {
               perror("atoi");
               exit(-1);
            }
            num_of_files = 3;
         }
         else {
            fprintf(stderr, "usage: %s [-n num] [file1 [file2 [...] ] ]\n", argv[0]);
            exit(-1);
         }
      }
   }

   init(); /* initializes our hashtable */

   while (num_of_files < argc) {
      /* go file by file, parsing the strings, and then adding those string to the hashtable.
       * this will be done using readstring(FILE *infile), which returns a string (word).
       * then we will use addtocount(char *s) which increments that word's count. */
      infile = fopen(argv[num_of_files], "r");
      while ((current_word = readstring(infile)) != NULL) {
         addtocount(current_word);
      }
      num_of_files++;
   }

   res = getlist(num_words);
   
   return(1);
}
