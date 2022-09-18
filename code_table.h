
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "text_func.h"
#ifndef __header__
#define __header__
	
	
	typedef struct table_row
	{
		struct table_row *next_row;
		struct word *head;
		struct word *last;
	} table_row;

	typedef struct word
	{
		struct word *next;
		char value[SYMBOL_SIZE];
		int address;
	}word;

	word *insert_word(char[SYMBOL_SIZE], table_row**, int, int);
	word* insert_word_after(word**, char[SYMBOL_SIZE], int);
	table_row* create_row();
	void freeAll(table_row**);
	void freeSubList(word**);
	void update(table_row**, int);
#endif
