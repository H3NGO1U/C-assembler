#include "helping.h"

/*remove spaces from begging and end*/
/*also used to check if the input is legal*/
/*returns the error signal*/
int remove_spaces(char* str, char (*newStr)[MAX_OP], int expected)
{
	
	int i = 0, k = 0, loc = start, err = DEF;
	char c;
	
	if(str != NULL)
	{
		
		while((c = str[i])!='\0' && k < MAX_OP-1)
		{
			
			if(loc == start && !isspace(c)) /*stops ignoring spaces*/
				loc = mid;
			if(loc == mid)
			{
				if(isspace(c))  /*a space after a non-space char*/
					loc = end;
				else
				{	
					(*newStr)[k] = c; /*copies char*/
					k++;
				}
			
			}
			if(loc == end && !isspace(c))
			{	
				err = MISS_COM;
				break;
			}
				
			i++;
		}/*end of while*/
	
	} /*end of if*/
	
	if(c != '\0' && err != MISS_COM) /*too many characters*/
	{
		err = TOO_LONG;
	}
	else if(loc == start) /*there wasnt characters except maybe spaces*/
	{
		if(!expected) /*expects to find characters*/
		{
			err = MISS_ARGS;
			
		}
	}
	else if(expected) /*expects not to find characters*/
	{
		err = TOO_MANY_ARGS;
	}
	
	(*newStr)[k] = '\0'; /*terminate string*/
	return err;
	
	
}

/*prints error messages*/
void printError(int error, int lineCount)
{
	
	if(error == MISS_COM)
		printf("\nin line %d: missing comma\n", lineCount);

	else if(error == TOO_MANY_ARGS)
		printf("\nin line %d: extraneous text after the end of command\n", lineCount);

	else if(error == MISS_ARGS)
		printf("\nin line %d: missing args\n", lineCount);
	else if(error == TOO_LONG)
		printf("\nin line %d: operand name is too long\n", lineCount);
	else if(error == ILLEGAL_STRING)
		printf("\nin line %d: illegal string\n", lineCount);
	else if(error == NOT_NUM)
		printf("\nin line %d: argument is not a real number\n", lineCount);
	else if(error == ILLEGAL_LBL)
		printf("\nin line %d: illegal label name\n", lineCount);
	else if(error == ILLEGAL_STRCT)
		printf("\nin line %d: not a legal struct member\n", lineCount);
	else if(error == ALREADY_DEFINED)
		printf("\nin line %d: a label with that name has ALREADY been defined\n", lineCount);

	else if(error == ILLEGAL_LBL_DEF)
		printf("\nin line %d: illegal label definition\n", lineCount);
	
	else if(error == DEFINED_EXT)
		printf("\nin line %d: label was already defined as extern\n", lineCount);
	else if(error== ENT_UNDEFINED)
		printf("\nin line %d: label defined as entry does not has definition\n", lineCount);
	else if(error ==SERIOUS_ERR)
	{
		printf("\nMEMORY LEAK\n");
		exit(1);
	}
}

/*cut the string from both edges and check if it is legal*/
int createString(char **string_buffer, char *token)
{
	int point = 0, is_string;
	point = skipper(token, SPACES, 0);
	*string_buffer = strdup(token+point); /*begins from " if legal string*/

	is_string = isString(*string_buffer);
	if(is_string)
		return DEF;
	return ILLEGAL_STRING;
}
	
/*checks if it is a legal string*/
int isString(char *str)
{
	char c;
	int i = 0;
	if(strlen(str) <= 1)
		return FALSE;
	while(str[i]!='\0')
	{
		if(!isspace(str[i])) 
			c = str[i];
		i++;
	}
	return str[0] == '"' && c == '"';
}
/*recieves a string clean from spaces*/
/*checks if this string is a decimal number*/
/*uses isdigit() function*/
/*returns 1 if it is a number, else 0*/
int isDigit(char *str)
{
	int i  = 0;
	char c;
	if(str == NULL) return 0;

	while((c=str[i])!='\0')
	{
		if(i != 0 ||( c != '-'&& c!= '+')) /*enter if you are not the first digit, or if you are not the sign*/
		{
			
			if(!isdigit(c))
				return 0;
		}
		i++;
	}
	
	return 1;
}

/*recieves a string clean from spaces*/
/*checks if this string represents a legal register*/
/*returns the register number if it is a register, else -1*/
int isRegister(char *str)
{
	int num;
	if(strlen(str)==2 && str[0] == 'r'  && isdigit(str[1]))
	{
		num = atoi(str+1);
		if(num < 8 && num>=0)
			return num;
	}

	return -1;
}


/*check if there are any arguments at the end of command*/
/*if there is, return TOO_MANY_ARGS*/
int check_end_command(char *token)
{
	char new_str[MAX_OP] = {0};
	token = strtok(NULL, "\0");
	return remove_spaces(token, &new_str, SPACES); /*using the function for removing spaces*/ 
}

/*check if there are commas before the word starts*/
int checkCommasBeforeCommand(char *str)
{
	int i = 0;
	while((isspace(str[i]) || str[i] == ','))
		{  
			if(str[i] == ',')
			{
				return TRUE;
			}
			i++;
		}
	return FALSE;	
}

/*skip a certain sequence of characters: spaces or non-spaces*/
int skipper(char* str, int spaces, int point)
{
	if(spaces) /*skip space*/
	{
		while(isspace(str[point])) point++;/*save the point where the command word begins*/
	}
	
	else /*skip non-spaces*/
	{
		while(!isspace(str[point]) && str[point] != ',' && str[point]!='\0') point++;
		if(str[point]=='\0') point++;
	}
	return point;
				
		
}

/*copy a name of struct without the number*/
void copyStructName(char org[MAX_OP], char (*dest)[MAX_OP])
{
	char c;
	int i;
	while((c = org[i]) != '.') /*stop at the point*/
	{
		(*dest)[i] = org[i];
		i++;
	} 
	(*dest)[i] = '\0';
}

/*duplicate a certain string*/
char *strdup(char *s)
{
	char *p;
	p=(char *)malloc((strlen(s)+1)*sizeof(char));
	if(p!=NULL)
		strcpy(p,s);
	return p;
}

/*dinamic allocation*/
char* enlarge(char *ptr, int *capacity)
{

	char* newPtr = NULL;
	*capacity *= 2;
	newPtr = (char*)realloc(ptr,*capacity*sizeof(char));
	return newPtr;
}


