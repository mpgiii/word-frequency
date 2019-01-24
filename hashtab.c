#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct nlist {  /* nodes for hashtable */
   struct nlist *next;
   char *name;
   int count;
};

#define HASHSIZE 32768

static struct nlist **hashtab = NULL; /* the hashtable itself */

void init() {
   hashtab = (struct nlist*) calloc(HASHSIZE, sizeof(struct nlist));
   if (!hashtab) {
      exit(-1);
   }
}

unsigned hash(char *s) {  /* hashing function, from K&R */
   unsigned hashval;
   
   for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 31 * hashval;
   return hashval % HASHSIZE;
}

struct nlist *lookup(char *s) {  /* find the node corresponding to a character */
   struct nlist *np;
  
   for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
      if (strcmp(s, np->name) == 0) {
         return np;
      }
   }
   return NULL;
}

void insert(char *s) {   /* inserts new word into hashtable */
   struct nlist *np;
   np = (struct nlist*) malloc(sizeof(struct nlist));
   if (!np) {
      exit(-1);
   }
   strcpy(np->name, s);
   np->next = NULL;
   np->count = 1;
   hashtab[hash(s)] = np;
}

void addtocount(char *s) { /* increments count of node */
   struct nlist* np;
   if ((np = lookup(s)) != NULL)
      np->count += 1;
   else insert(s);
}

int comparewords(struct nlist* a, struct nlist* b) {
   int final;

   if (!a && !b)
      return 0;
   else if (!a) 
      return -1;
   else if (!b)
      return 1;
   else {
      final = a->count - b->count;
      if (!final)
         final = strcmp(a->name, b->name);
   }
   return final;
}

void print_words(struct nlist* a) {
   if (a) {
      printf("The current words contained in this list are:\n");
      for (; a; a = a->next)
         printf("%s, %d\n", a -> name, a -> count);
   }
   else {
      printf("There are no words contained in this list.");
   }
}

/*********** BELOW: DOES NOT YET SORT VALUES BEFORE RETURNING *********/

struct nlist **getlist(int n) {
   struct nlist **res;
   struct nlist *np;
   int i;

   res = (struct nlist *) malloc((n+1) * sizeof(struct nlist));
   if (!res) {
      perror("malloc");
      exit(-1);
   }

   for (i = 0; (np != NULL) && (i < n); np = np->next) {
      res[i] = np;
   }
   res[i] = NULL; /* list ends in a NULL to show end */
   return res;
}
