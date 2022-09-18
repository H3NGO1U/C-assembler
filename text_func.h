#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef __text_func__
#define __text_func__
#define SYMBOL_SIZE 3
#define WORD_SIZE 10
char* connect(const char *str1, const char *str2); 
int binaryToDecimal(char[WORD_SIZE+1]); /* get binary number and returns number in decimal*/
void decimalToBinary(int, char(*)[WORD_SIZE+1]);/* get decimal number and returns number in binary */
void binaryToSymbol(char[WORD_SIZE+1], char (*)[SYMBOL_SIZE]);/*get binary number and returns number in a symbols */
void decimalToSymbol(int, char (*)[SYMBOL_SIZE]);/*get decimal number and returns number in a symbols */
void reverseString(char[], char(*)[]);
#endif
