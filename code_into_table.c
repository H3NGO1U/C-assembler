#include "code_into_table.h"
void createCMD(table_row **row, int IC)
{
	char CMD[WORD_SIZE+1] = {0}, reversed[WORD_SIZE+1], symbol[SYMBOL_SIZE];
	int i, counter = 0, val;
	CMD[counter++] = '0';
	CMD[counter++] = '0';
	val = command.dest;
	for(i = 0; i < 2; i++)
	{
		if(val%2)
			CMD[counter++] = '1';
		else CMD[counter++] = '0';
		val/=2;
	}

	val = command.src;
	for(i = 0; i < 2; i++)
	{
		if(val%2)
			CMD[counter++] = '1';
		else CMD[counter++] = '0';
		val/=2;
	}

	val = command.cmd;
	for(i = 0; i < 4; i++)
	{
		if(val%2)
			CMD[counter++] ='1';
		else CMD[counter++] = '0';
		val/=2;
	}
	CMD[10] = '\0';
	reverseString(CMD, &reversed); /*reverse the string before translating*/
	binaryToSymbol(reversed, &symbol);
	(*row)->head = insert_word(symbol, row, TRUE, IC);
}

/*if the command is legal, this function is called to code the rest command word*/
void create_rest_cmd(table_row **row, char *commandLine, int opps, int label_def, int *IC)
{
	char new_str[MAX_OP] = {0};
	char *token;
	char value[WORD_SIZE+1] = {0}, reversed[WORD_SIZE+1], symbol[SYMBOL_SIZE];
	int x, i, opp = 1, operand, start, reg = FALSE, size = WORD_SIZE;
	int src = TRUE; /*if it is the src operand*/
	token = strtok(commandLine, " \t\v\f\r");
	while(opp <= opps && token != NULL)
	{
		token = strtok(NULL, ",\n");
		remove_spaces(token, &new_str, 0);
		start = 1;
		if(opp < opps) /*means opp = 1 and opps = 2, this is the SRC operand*/
			operand = command.src;
		
		else
		{
			src = FALSE;
			operand = command.dest;
		}

		if(operand == STRCT || operand == DIR) /*if something that has an address to be coded in second transition*/
			(*IC)++;
		
		if(operand == INST  || operand == REG ||operand == STRCT)
		{
			
			value[0] = '0';
			value[1] = '0';
			if(operand == STRCT)
				while(new_str[start++] != '.'); /*get to the point*/

			if(command.dest == REG && src && operand == REG)
				reg = TRUE;

			i = 2;
			if(operand == REG && src)
				i = 6;
			if(!reg && operand == REG && src) /*initialize with zeroes*/
				for(i = 0; i < 6; i++)
					value[i] = '0';
			
				
			x = atoi(new_str+start);
			
			/*second register out of two*/
			if(!src && reg)
				size = 6;
			for(; i<size; x>>=1)
			{
				if(x&01)	
					value[i] = '1';
				else value[i] = '0';
				i++;  
		
			}
		
			if(src || !reg) /*only if first register out of two, or something else*/
				value[i] = '\0';
			
			/*if not the src or not a register*/
			if(!reg || !src)
			{
				
				reverseString(value, &reversed); /*reverse the string before translating*/
				binaryToSymbol(reversed, &symbol);
				insert_word(symbol, row, FALSE, *IC);
				(*IC)++;
			}
			
		}
		src = FALSE;
		opp++;
		
		
	}
}

/*recieves a legal instruction and codes it into code table, base 32*/
void createInstWord(char *str, int type, char *string_buffer, table_row **row, int *DC)
{
	char *token = strtok(str, ",");
	char arg[MAX_OP], symbol[SYMBOL_SIZE];
	int num, i = 1;
	int head = TRUE; /*is head in the first word*/
	
	switch(type)
	{
		case DATA:
			while(token!=NULL)
			{
				remove_spaces(token, &arg, NON_SPACES);
				num = atoi(arg);
				decimalToSymbol(num, &symbol);
				if(head)
					(*row)->head = insert_word(symbol, row, head, *DC);
				else insert_word(symbol, row, head, *DC);
				(*DC)++;
				token = strtok(NULL, ",");	
				head = FALSE;
			}
			break;
		case STRING_DATA:
			string_buffer[strlen(string_buffer)-1] = '\0'; /*delete the last parenthesis*/
			while(string_buffer[i] != '\0')
			{
				decimalToSymbol(string_buffer[i], &symbol);
				if(head)
					(*row)->head = insert_word(symbol, row, head, *DC);
				else insert_word(symbol, row, head, *DC);
				(*DC)++;
				head = FALSE;
				i++;
			}
			decimalToSymbol(0, &symbol);
			if(head)
				(*row)->head = insert_word(symbol, row, head, *DC);
			else insert_word(symbol, row, head, *DC);
			(*DC)++;
			break;
		case STRCT_DATA:
		
			remove_spaces(token, &arg, 0);
			num = atoi(arg);
			
			decimalToSymbol(num, &symbol);
			(*row)->head = insert_word(symbol, row, TRUE, *DC);
			(*DC)++;
			string_buffer[strlen(string_buffer)-1] = '\0'; /*delete the last parenthesis*/
		
			while(string_buffer[i] != '\0')
			{
				decimalToSymbol(string_buffer[i], &symbol);
				insert_word(symbol, row, FALSE, *DC);
				(*DC)++;
				i++;
			}
			
			decimalToSymbol(0, &symbol);
			insert_word(symbol, row, FALSE, *DC);
			(*DC)++;	
			break;
	}
}

/*if the parameters of the instruction are legal, code them into table*/
int codeDataParams(char *str,int id, int illegal_end, int lineCount, int *DC, char *name, nlist *(*symbol_table)[HASHSIZE], table_row **row, int label_def,
nlist *(*entext_table)[HASHSIZE])
{
	char *token, arg[MAX_OP], *string_buffer, *copyStr = (char*)malloc(strlen(str)+1);
	char *type;
	int err = DEF, argsCounter = 0;
	nlist *np;
	int point = skipper(str, NON_SPACES, 0); /*skip the instruction name*/
	str+=point;
	if(str == NULL)
		return MISS_ARGS;
	
	strcpy(copyStr, str); /*create a copy*/
	
	switch(id)
	{
		case DATA:
			err = DEF;
			if(illegal_end)
				err = TOO_MANY_ARGS;
			token = strtok(str, ",");
			while(token!=NULL && !err)
			{
				
				err = remove_spaces(token, &arg, NON_SPACES);
				if(err == MISS_ARGS) /*blank space, check if it is the end*/
				{
					
					if(argsCounter) /*if there are args at all*/
						err = DEF; /*CANCEL the error*/
					
					break;
				
				}

				if(!isDigit(arg)) /*not a number, and not a blank space*/
				{
						err = NOT_NUM;
						break;
				}
				
						
				token = strtok(NULL, ",");
				argsCounter++;

			}
			
			if(!err)
				err = check_end_command(token);
			if(!err)
			{
				
				if(label_def) /*only if label*/
				{
					err = checkIfExtern(*entext_table, name);
					if(!err)				
						err = install(name, "data", symbol_table, *DC);
				}

				if(!err)
				{
					createInstWord(copyStr, id, string_buffer, row, DC);
				}
				
				
			}

			break;


		case STRING_DATA: 
			err = 0;
			err = createString(&string_buffer, str);
			if(!err)
			{
				if(label_def) /*only if label*/
				{
					err = checkIfExtern(*entext_table, name);
					if(!err)				
						err = install(name, "data", symbol_table, *DC);
				}
					
				if(!err)	
				{
					createInstWord(copyStr, id, string_buffer, row, DC);
					
					
				}
				
				
			}
			free(string_buffer);
				
			break; 


		case STRCT_DATA: 
			err = 0;
			token = strtok(str, ",");
			err = remove_spaces(token, &arg, NON_SPACES);
			if(!err)
			{
				if(isDigit(arg))
				{
					token = strtok(NULL, "\0");
					
					if(token != NULL)
						err = createString(&string_buffer, token);
					else err = MISS_ARGS;

					if(!err)
						err = check_end_command(token);

					if(!err) /*still not error*/
					{
							if(label_def) /*only if label*/
							{
								err = checkIfExtern(*entext_table, name);
								if(!err)
									err = install(name, "data", symbol_table, *DC);				
							}
							if(!err)
							{
								createInstWord(copyStr, id, string_buffer, row, DC);
							}
							else printError(err, lineCount);
							
						
					}
					if(err != MISS_ARGS) 
						free(string_buffer);
					
				}	
				else err = NOT_NUM;
			}
			break;

		case ENTRY:
			type = "entry";
		case EXTERN:
			if(id == EXTERN)
				type = "extern";
			token = strtok(str, space_delim);
			err = remove_spaces(token, &arg, NON_SPACES);
			if(!err)
			{
			
				err = check_end_command(token);
				if(!err)
				{
					
					if(id == EXTERN && (np = lookup(arg, *symbol_table))!=NULL)
						err = ALREADY_DEFINED;
					else err = install(arg, type, entext_table, 1);
				}
			}
		break;
			
	}
	free(copyStr);
	return err;
}

/* update the memory so the instructions start at 100, and the data start after instructions*/
void updateMemoryInSymbolTable(nlist *(*symbol_table)[HASHSIZE], int IC)
{
	int i;
	nlist **np;
	IC+=MEMORY_START;
	
	if((*symbol_table) == NULL)
		return;

	for(i = 0; i<HASHSIZE; i++) /*go through the table*/
	{
		np = &((*symbol_table)[i]);
		while((*np)!=NULL) 
		{
			if(!strcmp((*np)->defn,"data"))
				((*np)->address) += IC;
			else ((*np)->address) += MEMORY_START;
			np = &((*np)->next);	
		}
	
		
	}

}

/*update the code tables so the instructions start at 100, and the data start after instructions*/
void updateMemoryInCodeTables(table_row **data_head,table_row **head, int IC)
{
	update(data_head, IC+MEMORY_START);
	update(head, MEMORY_START);
}



/*code an address of label, add '10' ARE as relocatable*/
void codeAddress(char (*symbol)[SYMBOL_SIZE], int address)
{
	char value[WORD_SIZE+1], reversed[WORD_SIZE+1];
	int i;
	value[0] = '0';
	value[1] = '1';
	for(i = 2; i<WORD_SIZE+1; address>>=1)
	{
				
		if(address&01)	
			value[i] = '1';
		else value[i] = '0';
		i++;  
		
	}
	value[i] = '\0';
	reverseString(value, &reversed);
	binaryToSymbol(reversed, symbol);
	
}



