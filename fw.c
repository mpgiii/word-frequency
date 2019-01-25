/* 
 * fw.c - a program which takes in text files as
 * inputs (through stdio.h) and prints out the
 * most used words and their frequency to STDOUT.
 *
 * By default, the program will print out the 10
 * most common words and their frequency. If the
 * user would like to view more or fewer results,
 * the command line flag "-n" can be used.
 *
 * Usage: fw [-n num] [file1 [file2 ...] ]
 *
 * Michael Georgariou
 * 01/25/2019
 * CPE 357-01
 */

#include <stdlib.h>
#include <string.h>

#include "readstring.h"
#include "hashtab.h"

int main(int argc, char *argv[]) {
   FILE *infile;
   int num_words = 10; /* default word count */
   int num_of_files = 1;
   char *current_word;
   struct nlist *res;
   int num_word_unique = 0;
   int i;

   if (argc > 1) {
      /* if we have any arguments */
      if (!(strcmp(argv[1], "-n"))) {
         /* if the first argument is -n */
         if (argc > 2) {
            /* if there is another argument after the -n */
            num_words = atoi(argv[2]);
            if (num_words == 0) {
               fprintf(stderr, "usage %s [-n num] [file1 [file2 [...] ] ]\n", 
                       argv[0]);
               exit(-1);
            }
            if (num_words < 0) {
               fprintf(stderr, "%s: Negative words cannot be counted.\n", 
                       argv[0]);
               exit(-1);
            }
            /* set num_words to the second argument. Doesn't work? Exit. */
            num_of_files = 3;
            /* set num_of_files to start at 3 to account for extra arguments */
         }
         else {
            /* if there is no argument after the -n, exit. */
            fprintf(stderr, "usage: %s [-n num] [file1 [file2 [...] ] ]\n", 
                    argv[0]);
            exit(-1);
         }
      }
   }

   init();
   /* initialize our hash table to store our strings */
   if (num_of_files < argc) {
      while (num_of_files < argc) {
         /* go file by file, parsing the strings, adding them to the hashtable.
          * done using readstring(FILE *infile), which returns a string (word).
          * then use addtocount(char *s) which increments word's count. */
         infile = fopen(argv[num_of_files], "r");
         if (infile == NULL) {
             perror(argv[num_of_files]);
             num_of_files++;
             continue;
             /* if the file can't be opened, print why and continue */
         }
         current_word = readstring(infile);
         /* get the first word from the file */
         while (current_word != NULL) {
            /* while the file still has words in it */
            if (addtocount(current_word) == 0)
               /* if addtocount returns 0, it means this is a new word */
               num_word_unique++;
            current_word = readstring(infile);
            /* read the next word from the file */
         }

         fclose(infile);
         /* close the file after we are done */

         num_of_files++;
         /* move on to the next file */
      }
   }
   else {
      infile = stdin;
      current_word = readstring(infile);
      while (current_word != NULL) {
         if (addtocount(current_word) == 0)
            num_word_unique++;
         current_word = readstring(infile);
      }
   }

   res = getsortedlist(num_words);
   /* sort the list and store it */
   

   printf("The top %d words (out of %d) are:\n", num_words, num_word_unique);
   

   for (i=0; i < num_words; i++) {
      if (res != NULL) {
         printf("%9d %s\n", res->count, res->name);
         res = res->next;
         /* print the count and name of a node, then move onto the next.
          * if the next is NULL, we have reached the end and will stop early. */
      }
   }

   return(1);
} 
