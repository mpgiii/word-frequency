#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct nlist {  /* nodes for hashtable */
   struct nlist *next;
   char *name;
   int count;
};

#define HASHSIZE 9999999

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

unsigned long hash(char *s) {  /* hashing function, from K&R */
   unsigned long hashval;

   for (hashval = 0; s && *s; s++) {
      hashval = *s + 31 * hashval;
   }

   return hashval % HASHSIZE;
}

struct nlist *newnlist(char *s) {
   struct nlist *np;
   
   /* first, allocate space for both our pointer and the
    * string it will contain */
   np = (struct nlist*) malloc(sizeof(struct nlist));
   if (!np) {
      perror("malloc");
      exit(-1);
   }
   np->name = (char*) malloc(strlen(s));
   if (!np) {
      perror("malloc");
      exit(-1);
   }
   
   /* once memory is allocated, stick the string parameter
    * into the node, set its count to 1 (as this is the
    * first occurance), and set its tail to NULL (will be
    * set in another function, if needed. */
   strcpy(np->name, s);
   np->count = 1;
   np->next = NULL;
   return np;
   
}

int addtocount(char *s) {
   /*returns a 0 if word is not in hash table */
   int result = 0;
   int index;
   struct nlist *np;
   struct nlist *firstinhash;
   
   index = hash(s);
   firstinhash = hashtab[index];
   /*grab whatever is stored at the index in our hashtable
    *that corresponds to this string */
   
   for (np = firstinhash; np; np = np->next) {
      /* goes through all nlist's stored in this index.
       * if empty, for loop will do nothing */
      if (!strcmp(np->name, s)) {
         /* if we find an nlist with the string we are looking for,
          * add one to its count and set the result to 1 to
          * indicate the word was already in the hash table */
         np->count += 1;
         result = 1;
         break;
      }
   }
   
   
   
   if (np == NULL) {
      /* if the string we were looking for isn't already there */
      np = newnlist(s);
      /* create a new nlist with our string */
      if (firstinhash) {
         /* if there were values in this index of our hash table,
          * set this new nlist to the front of this index */
         np->next = firstinhash;
      }
      if (first == NULL) {
         /* if this is our first value inserted, set its tail
          * to null to indicate the end of our linked list */
         np->next = last;
      }
      else {
         /* if not, set its tail to the first item in the list */
         np->next = first;
      }

      first = np;
      /* then set the front of the linked list to this new node. */
      
      hashtab[index] = np;
      /* last but not least, stick that node into the hash table */
   }
   
   return result;
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

   if (a == NULL)
      return(b);
   else if (b == NULL)
      return(a);

   if (a->count > b->count) {
      result = a;
      result->next = SortedMerge(a->next, b);
   }

   else if (a->count == b->count) {
      if (strcmp(a->name, b->name) > 0) {
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
