#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "text_func.h"
#include "hash.h"
#include "defFuncs.h"
#define MAX_LEN 100

int macro_to_table(const char*);
int check_macro(char*);
void save_in_table(FILE *fp1,char *macro_name, nlist *(*)[HASHSIZE], int*);
void copy_from_as_to_am (const char *str1, nlist *[HASHSIZE]);
