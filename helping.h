
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef __helping__
#define __helping__
#define SIZE 100
#define MAX_OP 33
#define LINE_LEN 80
#define space_delim " \t\v\f\r\n"
#define INST_STRT -6
#define MEMORY_START 100
struct 
{

unsigned int ARE : 2;
unsigned int dest : 2;
unsigned int src : 2;
unsigned int cmd: 4;
} command;
enum Boolean
{
	FALSE, TRUE 
};  
enum LOC
{
	start, mid, end
};
enum {
	NON_SPACES, SPACES
};
enum ERROR
{
	DEF, UNDEFINED, MISS_COM, MISS_ARGS, TOO_MANY_ARGS, ILLEGAL_LBL, ILLEGAL_LBL_DEF, ILLEGAL_STRCT, TOO_LONG, ILLEGAL_STRING,
	 NOT_NUM, ALREADY_DEFINED,DEFINED_EXT, ENT_UNDEFINED, SERIOUS_ERR
};

enum ADDRESSING
{
	INST, DIR, STRCT, REG
};

enum LEGAL_OPPS
{
	NO_OPP, ALL, NO_NUM, NO_NUM_AND_REG
};
enum {
	DATA = -6, STRING_DATA, STRCT_DATA, ENTRY, EXTERN
};
int remove_spaces(char*, char(*)[MAX_OP], int);
void printError(int, int);
int createString(char**, char*);
int isString(char*);
int isDigit(char*);
int isRegister(char*);
int checkCommasBeforeCommand(char*);
int check_end_command(char*);
int skipper(char*, int, int);
void copyStructName(char[MAX_OP], char(*)[MAX_OP]);
char *strdup(char *s);

char* enlarge(char*, int*);
#endif
