#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "helping.h"
#ifndef __hash_header__
#define __hash_header__
#define HASHSIZE 101
#define MAX_OPP 31
typedef struct nlist{
	struct nlist *next;
	char name[MAX_OPP];
	char *defn;
	int address;	   
} nlist;

#endif
unsigned hash(char *s);
nlist *lookup(char *s, nlist(*[HASHSIZE]));
int install(char *name,char *defn, nlist(*(*)[HASHSIZE]) , int address);
void createTable(nlist *(*)[HASHSIZE]);
void display(nlist(*[HASHSIZE]));
void destroy(nlist(*(*)[HASHSIZE]));
