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

struct nlist *first = NULL; /* head of linked list */
static struct nlist **hashtab = NULL; /* the hashtable itself */
struct nlist *last = NULL; /* tail of linked list */


void init() {
   hashtab = (struct nlist**) calloc(HASHSIZE, sizeof(struct nlist));
   if (!hashtab) {
      perror("calloc");
      exit(-1);
   }
}

unsigned hash(char *s) {  /* hashing function, from K&R */
   unsigned long hashval;

   for (hashval = 0; s && *s; s++) {
      hashval = ((hashval<<2) + (*s) - 'a');
   }

   return hashval % HASHSIZE;
}

struct nlist *lookup(char *s) {  /* find the node for a character */
   struct nlist *np;
   
   if ((np = hashtab[hash(s)]) != NULL)
      return np;

   return NULL;
   
}

void insert(char *s) {   /* inserts new word into hashtable */
   
   struct nlist *np;
   
   np = (struct nlist*) malloc(sizeof(struct nlist));
   if (!np) {
      perror("malloc");
      exit(-1);
   }

   np->name = (char*) malloc(strlen(s) + 1);
   if (!(np->name)) {
      perror("malloc");
      exit(-1);
   }

   strcpy(np->name, s);
   np->count = 1;
   /* put the string into the node, with a count of 1 */
  
   if (first == NULL) {
      np->next = last;
   }

   else {
      np->next = first;
   }

   first = np;
   hashtab[hash(s)] = np;
   /* finally put the node into the table for easy access */
}

int addtocount(char *s) { /* increments count of node */
   struct nlist* np;
   /*if the node is in the table, increment its count and return a 1 */
   if ((np = lookup(s)) != NULL) {
      np->count += 1;
      return(1);
   }
   /* otherwise, insert it into the table and return a 0 */
   insert(s);
   return(0);
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


/*********** BELOW: SORTING ALGORITHM *********/
/* A mergesort algorithm, Worst/Average/Best case O(n log n) efficiency. */


void split(struct nlist* source, struct nlist** front, struct nlist** back) {
   struct nlist* fast;
   struct nlist* slow;

   slow = source;
   fast = source->next;

   while (fast != NULL) {
      fast = fast->next;
      if (fast != NULL) {
         slow = slow->next;
         fast = fast->next;
      }
   }

   *front = source;
   *back = slow->next;
   slow->next = NULL;
}

struct nlist* SortedMerge(struct nlist* a, struct nlist* b) {
   struct nlist* result = NULL;
   int strc;

   if (a == NULL)
      return(b);
   else if (b == NULL)
      return(a);

   if (a->count > b->count) {
      result = a;
      result->next = SortedMerge(a->next, b);
   }

   else if (a->count == b->count) {
      strc = strcmp(a->name, b->name);
      if (strc > 0) {
         result = a;
         result->next = SortedMerge(a->next, b);
      }
      else {
         result = b;
         result->next = SortedMerge(a, b->next);
      }
   }

   else {
      result = b;
      result->next = SortedMerge(a, b->next);
   }

   return(result);
}

void mergesort(struct nlist **start) {
   struct nlist* head = *start;
   struct nlist* a;
   struct nlist* b;
   
   if ((head == NULL) || (head->next == NULL)) {
      return;
   }

   split(head, &a, &b);

   mergesort(&a);
   mergesort(&b);

   *start = SortedMerge(a, b);
}

struct nlist *getsortedlist() {
   /* runs the mergesort algorithm on the list and returns the
    * head of the sorted list (the highest occurring word) */
   mergesort(&first);
   return first;
}
