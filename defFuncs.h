#ifndef __isLegal__
#define __isLegal__
#include "helping.h"
#include "code_into_table.h"

#define UNMATCHING_OPP_MSG printf("\nin line %d: unmatching operand\n", lineCount)
#define UNDEFINED_PROB_MSG printf("\nUndefined Problem")
#define ILLEGAL_COMMA_MSG printf("\nin line %d: illeggal comma\n", lineCount)
#define NOT_NUM_MSG printf("\nin line %d: argument is not a real number\n", lineCount)

int getOpps(int);
int getSrcOpp(int);
int getDestOpp(int);
int checkLabelType(char*);
int checkLabel(char*);
int checkCommand(char*);
int checkInstruction(char*);
int checkData(char*);
int checkIfExtern(nlist *[HASHSIZE], char*);

#endif
