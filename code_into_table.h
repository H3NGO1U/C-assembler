#include "helping.h"
#include "code_table.h"
#include "hash.h"
#include "defFuncs.h"
/*functions for managing different tables declared by the assembler*/
void createCMD(table_row**, int);
void create_rest_cmd(table_row**, char*, int, int ,int*);
void createInstWord(char*, int, char*, table_row**, int*);
int codeDataParams(char*, int,int, int, int*, char*, nlist(*(*)[HASHSIZE]), table_row**, int,  nlist(*(*)[HASHSIZE]));



void updateMemoryInSymbolTable(nlist(*(*)[HASHSIZE]), int);
void updateMemoryInCodeTables(table_row**,table_row**, int);
void codeAddress(char (*)[SYMBOL_SIZE], int);



