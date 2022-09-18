#include "defFuncs.h"

struct {
	char *name;
	int opps; /*number of opperands allowed*/
	int legal_src_opp; /*which source operands the command accepts*/
	int legal_dest_opp; /*which destination operands the command accepts*/
} COMMANDS[] = {
		{"mov", 2, ALL, NO_NUM}, 
		{"cmp", 2, ALL, ALL},
		{"add", 2, ALL, NO_NUM},
		{"sub", 2, ALL, NO_NUM},
		{"not", 1, NO_OPP, NO_NUM},
		{"clr", 1, NO_OPP, NO_NUM},
		{"lea", 2, NO_NUM_AND_REG, NO_NUM},
		{"inc", 1, NO_OPP, NO_NUM},
		{"dec", 1, NO_OPP, NO_NUM},
		{"jmp", 1, NO_OPP, NO_NUM},
		{"bne", 1, NO_OPP, NO_NUM},
		{"get", 1, NO_OPP, NO_NUM},
		{"prn", 1, NO_OPP, ALL},
		{"jsr", 1, NO_OPP, NO_NUM},
		{"rts", 0, NO_OPP, NO_OPP},
		{"hlt", 0, NO_OPP, NO_OPP}}; 




/*return number of opperands allowed*/
int getOpps(int i)
{
	return COMMANDS[i].opps;
}

/*return what opperands allowed as source opperand*/
int getSrcOpp(int i)
{
	return COMMANDS[i].legal_src_opp;
}

/*return what opperands allowed as destination opperand*/
int getDestOpp(int i)
{
	return COMMANDS[i].legal_dest_opp;
}

/*checks if a label is a struct or direct*/
int checkLabelType(char* str)
{
	char c;
	int i = 0, strct = FALSE, state = DIR; /*default state is a direct (ordinary) label*/
	while((c=str[i]) != '\0')
	{
		if(strct) /*a point was here before - looking for 1 or 2*/
		{
			if(state == STRCT || (c != '1' && c != '2')) /*defined already 1 or 2 and still have chars or c isnt 1 or 2*/
			{
				
				return ILLEGAL_STRCT;
			}	
				
			else
			 	state=STRCT; 
			
		}
		if(!isalnum(c))
		{
			if(c == '.')
			{
				strct = TRUE;
			}
			else
			{
		
				return ILLEGAL_LBL;
			}
			
		}
		i++;
	}
	

	return state;
	
}

/*checks if a name is legal*/
int checkLabel(char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		if(i == 0 && !isalpha(str[i]))
			return 0;
		else if(!isalnum(str[i]))
		{
			
			return 0;
		}
		i++;
	}
	if(isRegister(str)!=-1)
		return FALSE;
	if(checkCommand(str)!=-1)
		return FALSE;
	if(checkInstruction(str)!=-1)
		return FALSE;
	return TRUE;
}

/*define the command name, if undefined command return -1*/
int checkCommand(char *str)
{	
	int i;
	if(str!=NULL)
		for(i=0; i<16; i++)
			if(!strcmp(str, COMMANDS[i].name))
				return i;
	return -1;
}

/*check if it is instruction*/
int checkInstruction(char *str)
{
	int i;
	char *dataInstrc[5] = {"data", "string", "struct", "entry", "extern"};
	for(i = 0; i<5; i++)
		if(!strcmp(str, dataInstrc[i]))
			return i;
	return -1;
		
}

/*check the first word if it is instruction*/
int checkData(char *str)
{
	int inst;
	char *token, arg[MAX_OP];
	if(str[0] == '.')
	{
		str++;
		token = strtok(str, space_delim);
		remove_spaces(token, &arg, 0);
		inst = checkInstruction(arg);
		if(inst != -1)
			return inst+INST_STRT;/*start from negative*/
		
		
	}
	
	return UNDEFINED;
}

/*check if a label was defined as extern*/
int checkIfExtern(nlist *entext_table[HASHSIZE], char *name)
{
	int err = DEF;
	nlist *np = lookup(name, entext_table);
	if(np!=NULL)
	{
		if(!(strcmp(np->defn, "extern"))) 
			err = DEFINED_EXT;
	}
	return err;
}

