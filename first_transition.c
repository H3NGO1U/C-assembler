
#include "first_transition.h"


/*recieves the command*/
/*checks which command is this*/
void intrprtr(const char *filename)
{
	char *commandLine, *tempLine; /*temp for increasing dinamic commandLine*/
	char *token, *copyLine, *secondCopy;
	char *label_name = (char*) malloc(MAX_OP * sizeof(char));
	char c = 0;
	int loc, i, err = 0,errCommas = 0, lineCount = 0; 
	int label_def = FALSE;    
	int capacity, foundComma, pos;  
	int general_error = FALSE; /*if found ANY error - dont move to second transition*/
	int IC = 0, DC = 0; /*COUNTERS: IC for instructions counter, DC for data counter*/ 
	table_row *head = create_row(), *temp = create_row();
	table_row *data_head = create_row(), *data_temp = create_row(); 
	nlist *symbol_table[HASHSIZE]; 
	nlist *entext_table[HASHSIZE]; 

	char *fileam = connect(filename, ".am\0");
	
	FILE *file = fopen(fileam, "r");
	free(fileam);
	if(file == NULL)
	{
		printf("\nfailed to open file\n");		
		exit(1);
	}
	
	createTable(&symbol_table);
	createTable(&entext_table);
	
	
	head->next_row = temp;
	data_head->next_row = data_temp;
	while(c!=EOF)
	{
		
		i = 0; 
		capacity = SIZE;
		commandLine = (char*)malloc(capacity*sizeof(char));
		loc = start;
		errCommas = 0;
		label_def = FALSE;
		/*recieve the command line into commandLine*/
		
		while((c = commandLine[i++] = fgetc(file))!=EOF) 
		{
			
			if(i == capacity -1) /*allocate more space*/
			{
				tempLine = enlarge(commandLine, &capacity);
				if(tempLine!=NULL)
					commandLine = tempLine;
				else
				{
					UNDEFINED_PROB_MSG;
					free(commandLine);
					exit(1);
				}
			
			}
			

			if(c == '\n')
			{
				lineCount++;
				if(loc==mid)
					break; /*end of command is '\n' after a non empty line*/
				
			}
			
		
			if(loc == start && !isspace(c))
				loc = mid; /*if we get enter in mid stage, it would be the end of command*/			
			
		} /*end of command recieve*/

		commandLine[i-1] = '\0'; /*terminate*/
		
		pos = skipper(commandLine, SPACES, 0); /*skip spaces at the begging*/
		if(commandLine[pos] == ';')
		{
			continue; /*ignore comments*/
		}
		/*check before the first word for commas, no comma is allowed*/
		
		if(checkCommasBeforeCommand(commandLine))	
		{
			ILLEGAL_COMMA_MSG;
			general_error = TRUE;
			continue;
		}

		i = skipper(commandLine, NON_SPACES, pos); /*skip the first word*/
		if(commandLine[i-1] == ':')
		{
				if(i - pos >= MAX_OP)
				{
					printError(TOO_LONG, lineCount);
					general_error = TRUE;
					continue;
				}
				commandLine[i-1] = ' '; /*remove the colon from the name*/
				label_def = TRUE;
		}
		
		
		if(label_def)
		{
			
			if(checkCommasBeforeCommand(commandLine+i))	
			{
				ILLEGAL_COMMA_MSG;
				general_error = TRUE;
				continue;
			}
			
			i = skipper(commandLine, SPACES, i); /*skip spaces after label name*/
			i = skipper(commandLine, NON_SPACES, i); /*skip the second word: command*/
		}
		
		/*check the whole line for illegal commas*/
		
		
		loc = start;
		foundComma = 0;
		while(commandLine[i]!='\0')
		{
			if(commandLine[i] == '"')
				break; /*inside a parenthases commas are allowed*/
			if(commandLine[i] == ',')
			{
				if(loc == start)
				{	
					ILLEGAL_COMMA_MSG; /*comma after command is forbidden*/
					errCommas = 1;
					break;
				}
				if(!foundComma)
					foundComma = 1;
				else /*comma was here before*/
				{
					printf("\nin line %d: illegal consecutive commas\n", lineCount);
					errCommas = 1;
					break;
				}
			
			}
			else if(!isspace(commandLine[i]))
			{
				loc = mid;
				foundComma = 0;
			}
			i++;
		} /*end of commas check*/
		if(errCommas)
		{
			general_error = TRUE;
			continue;
		}
		copyLine = (char*) malloc(capacity*sizeof(char)); /*allocate enough memory*/
		secondCopy = (char*) malloc(capacity*sizeof(char));
		if(copyLine == NULL || secondCopy == NULL)
		{
			UNDEFINED_PROB_MSG;
			exit(1);
		}
			
		strcpy(copyLine, commandLine);	
		strcpy(secondCopy, commandLine);
		token = strtok(commandLine+pos, space_delim); /*cut the command or label name*/
		
		if(token == NULL)
		{
			continue;
		}
		/*if it is a label definition*/
		if(label_def) /*recognized before as ending with colon: suspected to be label*/
		{
			
			if(checkLabel(token)) /*if legal label name*/
			{
				pos = skipper(copyLine, NON_SPACES, pos); /*skip the name*/	
				pos = skipper(copyLine, SPACES, pos); /*skip the spaces, get to the begining of content*/
				strcpy(label_name, token);
				token = strtok(NULL, space_delim);
			
				
			}
			else 
			{     
					printError(ILLEGAL_LBL, lineCount);
					general_error = TRUE;
					continue;
			} 
		}
		
		if((i = checkCommand(token))!= -1)
		{
			
			command.cmd = i;
			err = intrprtrOps(token, i, lineCount, foundComma); /*if foundComma is true, there is a COMMA in the end of command*/
			
			if(!err)
			{
				if(label_def)
				{
					err = checkIfExtern(entext_table, label_name); /*can't be extern and defined in this file*/
					if(!err)
						err = install(label_name, "instruction", &symbol_table, IC); /*defined an instruction label*/
						
					printError(err, lineCount);
					if(err)
						general_error = TRUE;
				}

				createCMD(&temp, IC); /*create the first word*/
				IC++;
				create_rest_cmd(&temp, copyLine+pos, getOpps(i), label_def, &IC); /*create the additional words and promote IC*/
				
		
				
				
			}
			else general_error = TRUE;
		}

		else
		{
				
				err = checkData(copyLine+pos);
				
				if(err == UNDEFINED) /*NOT data instruction: not .string, .struct or .data*/
				{
					general_error = TRUE;
					if(label_def)
					{
						printError(ILLEGAL_LBL_DEF, lineCount);
					}
					else printf("\nin line %d: undefined command name\n", lineCount);

				}
				else 
				{
				err = codeDataParams(secondCopy+pos, err, foundComma, lineCount, &DC, label_name, &symbol_table, &data_temp, label_def, &entext_table);
					
						if(err)
						{
							printError(err, lineCount);
							general_error = TRUE;
							
						}
						
					} 
				
			}
		
		free(copyLine);
		free(secondCopy);
		free(commandLine);
		temp->next_row = create_row();
		temp = temp->next_row;

		data_temp->next_row = create_row();
		data_temp = data_temp->next_row;
	
	} /*end of outer while*/
	updateMemoryInSymbolTable(&symbol_table, IC);
	updateMemoryInCodeTables(&data_head,&head, IC);

	if(!general_error)
		final_code(file, filename, symbol_table,entext_table, data_head, &head, IC, DC);


	destroy(&symbol_table);
	destroy(&entext_table);

	freeAll(&head);
	freeAll(&data_head);
	
	free(label_name);
	

} 

int intrprtrOps(char *token, int comd_num, int lineCount, int illegal_end)
{
	char new_str[MAX_OP] = {0};
	int err = 0, reg_num = -1, label, opp = 1, legal, src;
	int opps = getOpps(comd_num);
	command.src = 0;
	command.dest = 0;
	while(token != NULL && opp <= opps && !err)
	{
		token = strtok(NULL, ",\0");
		if(opp == 1 && opps == 2) /*one operand out of two means it is the src operand*/
		{
			src = 1;
			legal = getSrcOpp(comd_num);
		}
		else
		{
			 src = 0;
			 legal = getDestOpp(comd_num);
		}

		err = remove_spaces(token, &new_str, NON_SPACES);
		
		if(!err)
		{
			if(new_str[0] == '#')
			   {
				
				if(isDigit(new_str+1))
				{
					if(legal == ALL)
					{
						if(src)
							command.src = INST;
						else command.dest = INST;
					}
					else
					{
						UNMATCHING_OPP_MSG;
						err = 1;
					}
					
				}
				else err = NOT_NUM;
			} /*end of check digit*/
		
			else if(isalpha(new_str[0]))
			{
				if(new_str[0] == 'r')
				{
					reg_num = isRegister(new_str);
					if(reg_num!=-1)
					{
						if(legal == ALL || legal == NO_NUM)
						{
							if(src)
								command.src = REG;
							else command.dest = REG;
						}
						else
						{
							UNMATCHING_OPP_MSG;
							err = 1;
						}
					}
				}
				if(reg_num == -1)
				{
					label = checkLabelType(new_str); /*DIR if ordinary label, otherwise STRCT*/
					 
					if(label!=ILLEGAL_LBL && label!=ILLEGAL_STRCT)
					{
						
						if(legal != NO_OPP)
						{
							if(src)
								command.src = label;
							else command.dest = label;
						}
						else
						{
							UNMATCHING_OPP_MSG;
							err = 1;
						}
					}
					else err = label;
				}
			} /*end of check register or label*/

			else 
			{
				
				err = ILLEGAL_LBL;
			}

			reg_num = -1;
			opp++;
		} /*end of if not err*/

		if(err)
		{ 
			printError(err, lineCount);
			return err;
		}
			
	} /*end of while*/
		
		
		if(token!=NULL && !err)
			err = check_end_command(token);
		
		if(illegal_end) /*illegal end means comma at end of command*/
			err = TOO_MANY_ARGS;
		
		printError(err, lineCount);
		return err;
}


