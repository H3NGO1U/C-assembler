#include "code_table.h"

/*sets a new base 32 word into a table row*/
word* insert_word(char *symbol, table_row **row,  int head, int address) /*head or not head*/
{
	word *new_word;	
	new_word = (word*)malloc(sizeof(word));
	
	if(new_word == NULL)
		return NULL;

	strcpy(new_word->value,symbol); /*copies the desired value to the space in the table*/
	new_word->address = address;
	new_word->next = NULL;
	
	if(!head) /*if it is not the head, connect the word to the list*/
		((*row)->last)->next = new_word;

	((*row)->last)=new_word; /*set the last in the list to be the current word*/

	return new_word;
}

/*insert word in the second transition*/
word* insert_word_after(word **this_word, char symbol[SYMBOL_SIZE], int address)
{
	word *temp = (*this_word)->next;
	word *new_word = (word*)malloc(sizeof(word));
	
	if(new_word == NULL)
		return NULL;
	
	strcpy(new_word->value,symbol); /*copies the desired value to the space in the table*/
	new_word->address = address;
	
	(*this_word)->next = new_word;
	new_word->next = temp;
	return new_word;
}
/*create a new row in the table, each row is for a new command line*/
table_row* create_row()
{
	table_row *new_row;
	new_row = (table_row*)malloc(sizeof(table_row));
	new_row->head = NULL;
	new_row->last = NULL;
	new_row->next_row = NULL;
	return new_row;
}



/*frees all allocated memory for the code table*/
void freeAll(table_row **headRow)
{
	word *head;
	if((*headRow) == NULL)
		return;
	head = (*headRow)->head;
	freeSubList(&head);
	freeAll(&((*headRow)->next_row));
	(*headRow)->next_row = NULL;
	(*headRow)->head = NULL;
	(*headRow)->last = NULL;
	free(*headRow);
	(*headRow) = NULL;
}

/*helping function, frees each row's words*/
void freeSubList(word **head)
{
	if((*head) == NULL)
		return;
	freeSubList(&((*head)->next));
	(*head)->next = NULL;
	free(*head);
	(*head) = NULL;
}

/*adds to the addresses the val requested*/
void update(table_row** head, int val)
{	
	table_row *row = (*head)->next_row; /*move to next row - first is just a pointer (empty line)*/
	word* word;
	while(row!=NULL)
	{
		word = row->head;
		while(word!=NULL)
		{
			word->address += val;
			word = word->next;
		}
		row = row->next_row;
	}
}


