#include "helping.h"
#include "code_table.h"
#include "defFuncs.h"
typedef struct externs
{
		struct externs *next;
		char *name;
		int counterVal;
} externs;

void final_code(FILE*,const char*, nlist (*[HASHSIZE]), nlist (*[HASHSIZE]), table_row*, table_row**, int, int);
int checkEntry(char*, nlist (*[HASHSIZE]));
void updateInEntryFile(nlist(*[HASHSIZE]),nlist(*[HASHSIZE]),const char*);
void updateInExternFile(externs*,const char*);
void updateInObjectFile(const char*, table_row*, table_row*, int, int);
void writeToOb(FILE*, table_row*);
externs* createList();
void destroyList(externs**);

