#include "second_transition.h"

void final_code(FILE *file, const char *filename, nlist *symbol_table[HASHSIZE], nlist *entext_table[HASHSIZE], table_row* data_head, table_row** cmd_head, int IC, int DC)
{
	char *commandLine, *tempLine; /*temp for increasing dinamic commandLine*/
	char *token, strctArg[MAX_OP], arg[MAX_OP];
	char symbol[SYMBOL_SIZE];
	char c = 0;
	int loc, i, err = 0, lineCount = 0; 
	int labelType;    /*struct or direct label*/
	int capacity, pos;  
	int general_error = FALSE; /*if found ANY error - dont move to second transition*/
	int opps, comd_num, ent = FALSE, ext = FALSE;
	nlist *np;

	table_row *temp = (*cmd_head)->next_row;
	
	externs *extern_head = createList(), *extern_temp = createList(); /*list for externs*/
	
	word *temp_word;
	extern_head->next = extern_temp;
	rewind(file);
	
	while(c!=EOF)
	{
		
		i = 0; 
		capacity = SIZE;
		commandLine = (char*)malloc(capacity*sizeof(char));
		loc = start;
		temp_word = temp->head;
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
		
		token = strtok(commandLine, space_delim); /*label_name or command*/
		remove_spaces(token, &arg, NON_SPACES);
		if(arg[strlen(arg)-1] == ':')
		{
			token = strtok(NULL, space_delim); /*skip label name*/
			remove_spaces(token, &arg, NON_SPACES);
		}

		
		if(!strcmp(arg, ".entry")) /*if it is entry check if defined*/
		{

			ent = TRUE;
			token = strtok(NULL, space_delim); /*skip word .entry*/
			remove_spaces(token, &arg, NON_SPACES);
			err = checkEntry(arg, symbol_table); /*check if entry has definition*/
			
			if(err)
			{
				general_error = TRUE;
				printError(err, lineCount);
			}
		}

		
		else if((comd_num = checkCommand(arg))!=-1) /*if command*/
		{
			opps = getOpps(comd_num); /*get number of opperands*/
			token = strtok(NULL, ",");
			err = DEF;
			while(opps > 0 && token!=NULL &&!err)
			{	
				remove_spaces(token, &arg, NON_SPACES);
				if  (((labelType = checkLabelType(arg)) == STRCT) || checkLabel(arg))
				{
					
					if(labelType == STRCT)
					{
						copyStructName(arg, &strctArg); /*take the name until the point*/
						np = lookup(strctArg, symbol_table);
					}
					else np = lookup(arg, symbol_table); /*if direct label, look for it in the table*/
					
					if(np == NULL)/*maybe this is extern*/
					{
						np = lookup(arg, entext_table); /*check if exits in table*/
						
						if(np!=NULL && !strcmp(np->defn, "extern")) /*if exits and also is extern*/
						{
							ext = TRUE;
							extern_temp->counterVal = (temp_word->address)+1; /*sets the address to be current IC*/
							strcpy(extern_temp->name, arg); /*save the name*/
							extern_temp->next = createList();/*create next node in extern list*/
							extern_temp = extern_temp->next;/*move to next node*/
							decimalToSymbol(np->address, &symbol);/*convert the address of the label to 32 base (external label)*/
							insert_word_after(&temp_word, symbol,(temp_word->address)+1); /*insert into code table*/
							
						}
						else
						{
							printf("\nERROR:  undefined label in line %d\n", lineCount);
							general_error = TRUE; 
							err = TRUE;
						}
					}
					else /*label is defined, take its address*/
					{
						codeAddress(&symbol, np->address); /*add ARE*/
						
						insert_word_after(&temp_word, symbol, (temp_word->address)+1); /*insert into code table*/
						
					}
					
				}
				token = strtok(NULL, ","); /*move to next token*/ 
	
				if(!err)
				{
					if(labelType == STRCT && temp_word!=NULL)
						temp_word = temp_word->next; /*there are two code words, skip both*/
					if(temp_word!=NULL)
						temp_word = temp_word->next;
				}
				
				
				opps--;
			} /*end of while of command code completement*/
		} /*end of if*/
		
		temp = temp->next_row;
		free(commandLine);
	} /*end if file traverse*/
	
	
	if(!general_error)
	{
		if(ent) /*if there are entry defs*/
			updateInEntryFile(symbol_table, entext_table, filename);
		if(ext) /*if there are extern defs*/
			updateInExternFile(extern_head, filename);
		
		updateInObjectFile(filename, data_head, *cmd_head, IC, DC);
	}
	
	destroyList(&extern_head);
	
	fclose(file);
	
}

/*check if has definition in symbol table*/
int checkEntry(char *name, nlist *symbol_table[HASHSIZE])
{
	nlist *np = lookup(name, symbol_table);
	if(np == NULL)
		return ENT_UNDEFINED;
	else
		return DEF;
}

/*there are entry labels, create .ent file*/
void updateInEntryFile(nlist *symbol_table[HASHSIZE],nlist *entext_table[HASHSIZE],const char * filename)
{
	int i;
	nlist *np, *np2;
	int add;
	FILE *fp2;
	char *fileName1= NULL ,symbol[SYMBOL_SIZE];;
	fileName1 = connect(filename,".ent\0");
	if((symbol_table) == NULL)
		return;
	if((entext_table) == NULL)
		return;
	fp2 = fopen(fileName1, "w");
	free(fileName1);
	if(fp2 == NULL)
		exit(1);
	for(i = 0; i<HASHSIZE; i++) /*go through the table*/
	{
		np = (entext_table)[i];

		if((np)!=NULL && !strcmp(np->defn, "entry"))  /*write all entries into file.ent*/
		{
			fputs((np)->name,fp2);
			fputs("\t",fp2);

			np2 = lookup(np->name, symbol_table); /*look for the address in symbol table*/
			add=(np2)->address;
			decimalToSymbol(add, &symbol); /*convert it to 32 base*/
			fputs(symbol,fp2);
			fputs("\n",fp2);		
		}
	}
	printf("\nentry file successfully created\n");
	fclose(fp2);

}

/*there are extern labels, create .ext file*/
void updateInExternFile(externs *head,const char * filename)
{
	FILE *file;
	char *fileName1 = connect(filename,".ext\0");
	int add;
	char symbol[SYMBOL_SIZE];
	file = fopen(fileName1, "w");
	free(fileName1);
	if(file == NULL)
		exit(1);
	head = head->next;
	while((head->next)!=NULL)
	{
		fputs(head->name, file);
		fputs("\t",file);
		add=head->counterVal;
		decimalToSymbol(add, &symbol);
		fputs(symbol,file);
		fputs("\n",file);
		head = head->next;
				
	}
	printf("\nextern file successfully created\n");
	fclose(file);
}

/*no errors - create an object file*/
void updateInObjectFile(const char* filename, table_row *data_head, table_row *cmd_head, int IC, int DC)
{

	FILE *fp2;
	char *fileName1= NULL ,symbol[SYMBOL_SIZE];
	
	fileName1 = connect(filename,".ob\0");
	fp2 = fopen(fileName1, "w");
	free(fileName1);
	
	if(fp2 == NULL)
		exit(1);
	
	
	fputs("\n", fp2);

	decimalToSymbol(IC, &symbol);
	fputs(symbol, fp2);
	fputs("\t", fp2);

	decimalToSymbol(DC, &symbol);
	fputs(symbol, fp2);
	fputs("\n\n", fp2);

	writeToOb(fp2, cmd_head);
	writeToOb(fp2, data_head);
	
	printf("\nobject file successfully created\n");
	fclose(fp2);
}

/*take a code table and write it into an object file*/
void writeToOb(FILE *file, table_row *head)
{
	table_row *row;
	word *word;
	char symbol[SYMBOL_SIZE];
	row = head->next_row; /*move to next row - first is just a pointer (empty line)*/
	
	while(row!=NULL)
	{
		
		word = row->head;
		while(word!=NULL)
		{
			decimalToSymbol(word->address, &symbol);
			fputs(symbol, file);
			fputs("\t", file);
			fputs(word->value, file);
			fputs("\n", file);
			word = word->next;
		}
		row = row->next_row;
	}
}
/*create a new node, and allocate memory*/
externs* createList()
{
	externs *new_node;
	new_node = (externs*)malloc(sizeof(externs));
	new_node->name = (char*) malloc(sizeof(char)*MAX_OP);
	new_node->next = NULL;
	return new_node;
}

/*free the list*/
void destroyList(externs **head)
{
	if((*head) == NULL)
		return;
	destroyList(&((*head)->next));
	free((*head)->name);
	free(*head);
}


