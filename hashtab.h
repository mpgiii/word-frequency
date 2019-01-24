#ifndef hashtabh
#define hashtabh

struct nlist {
   struct nlist *next;
   char *name;
   int count;
};

extern void init();
extern void addtocount(char *s);
extern struct nlist *getlist(int n);

#endif
