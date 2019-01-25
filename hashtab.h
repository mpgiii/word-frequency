#ifndef hashtabh
#define hashtabh

struct nlist {
   struct nlist *next;
   char *name;
   int count;
};

extern void init();
extern int addtocount(char *s);
extern struct nlist *getsortedlist();

#endif
