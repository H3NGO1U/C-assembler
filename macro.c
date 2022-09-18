#include "macro.h"



/* check if found "macro" */
int check_macro(char* line){
    char *token = strtok(line, "\t ");
    if (strcmp(token,"macro") == 0)
    		return 1;
    
    else 
     	return 0;  /* if a macro not found */
    
}


int macro_to_table(const char *fileName)
{
	int general_error = 0;
	int pos = 0;
	char line[MAX_LEN];
	char *token = strtok(line, "\t ");
	int its_mac=0;
	nlist *macro_table[HASHSIZE];
	int line_num = 0;
	
    	char *macro_name = (char*)malloc(sizeof(char)*MAX_LEN);
	FILE *fp1;
	char *fileas = connect(fileName, ".as\0");
	createTable(&macro_table);
	
	fp1 = fopen(fileas, "r");
	free(fileas);
	if(fp1 == NULL)
	{
		printf("Error: can not open the file or the file is not exist: %s\n" , fileName);  	
		return 1;
	}
	if(macro_name==NULL)
	{
		printf("Error: could not allocate memory!\n");
			exit(1);
	}
	
	while (fgets(line,sizeof(line),fp1) != NULL)
	{
		line_num++;
		pos = skipper(line, SPACES, 0); /*skip spaces*/
		
		token = strtok(line+pos, space_delim);
		if(token == NULL)
			continue;
		
		if(strcmp(token,"macro") == 0)
		{

			its_mac=1;
		
			pos = skipper(line, SPACES, pos+6);
			token = strtok(line+pos, space_delim);
	
			if(token == NULL)
			{
				printf("\nERROR:in line %d: missing macro name\n",line_num);
				general_error = 1;
				continue;
			}
			
	 		strcpy(macro_name,token);
		
			if(check_end_command(token))
			{
				printf("Error- in line %d: too many names to one macro\n",line_num);
				general_error = 1;
			}
			
			/*checking  illegal macro name*/
			

			if(!checkLabel(macro_name))
			{
				printf("\nERROR: in line %d: found illegal macro name\n", line_num);
				general_error = 1;
			}
			
			
			if((lookup(macro_name, macro_table))!=NULL)
			{
				printf("\nERROR: in line %d: found a macro name that already exist\n", line_num);
				general_error = 1;
			}
			
		} /*end of if*/
		
		
		/*if found minimum 1 macro*/	
			
		if(its_mac==1)
		{
			save_in_table(fp1,macro_name, &macro_table, &general_error);
			its_mac=0;
		}
			
	}/*end of while*/
		
	if(!general_error)
	{
		fclose(fp1);
		copy_from_as_to_am(fileName, macro_table);
	}
		
	destroy(&macro_table);
	free(macro_name);
	return general_error;
}

/*save the macro with name and data in hash table*/
void save_in_table(FILE *fp1,char *macro_name, nlist *(*macro_table)[HASHSIZE], int *general_error)
{
	unsigned int i = 0;
	int pos1 = 0;
    	unsigned int j = 0;
	char line[MAX_LEN];
	char copyLine[MAX_LEN];
    	char* macro_data=(char*)malloc(sizeof(char)*MAX_LEN);
	char *token;
	int line_num=0;
	
	if(macro_data==NULL)
	{
		printf("Error: could not allocate memory!\n");
		exit(1);
	}

	while (fgets(line,sizeof(line),fp1)!=NULL)
    	{	
		line_num++;
		strcpy(copyLine, line);
		pos1 = skipper(line, SPACES, 0);
		token = strtok(line+pos1, space_delim);
		
		if(token == NULL)
			continue;	
		if(strcmp(token, "endmacro") == 0)
		{  
			if(check_end_command(token)) /*error*/
			{
				printf("\nERROR:in line %d extraneous text after endmacro token\n",line_num);
				*general_error = 1;
				
			}
			break;
		}
		i = 0;
		
		while(copyLine[i]!='\0')
        	{
			macro_data[j]=copyLine[i];
			++j;
			++i;
			
		}
		
		
		
	}
	macro_data[j] = '\0';
	install(macro_name,macro_data, macro_table, 0);
	free(macro_data);

}

/*make a .am file with all the macro data we save in the table*/
void copy_from_as_to_am (const char *fileName, nlist *macro_table[HASHSIZE])
{
	char line[MAX_LEN];
	char line2[MAX_LEN];
	FILE *fp1;
	FILE *fp2;
	char *token;
	int macro=FALSE, pos=0;
	nlist *np;
	char *fileName1= NULL;
	char *fileName2= NULL;
	fileName1 = connect(fileName, ".as\0");
	fileName2 = connect(fileName, ".am\0");
	fp1 = fopen(fileName1, "r");
	fp2 = fopen(fileName2, "w");

	printf("%s is saved as %s\n",fileName1,fileName2);
	free(fileName1);
	free(fileName2);
	while (fgets(line,sizeof(line),fp1) != NULL)
	{
      		if (check_macro(line))
 		{
			macro=TRUE; /*there are macros*/
		}

	}

	rewind(fp1);
	/*no macro all you need is copy the text from a .as file to a .am file*/
	if(macro==FALSE)
	{
		
		while (fgets(line,sizeof(line),fp1) != NULL)
		{
			fputs(line,fp2);
		}
		
  		
	}
/*need to take data macro from the table*/
	else 
	{

		while(fgets(line,sizeof(line),fp1)!=NULL)
		{
			
			strcpy(line2,line);
			
			pos = skipper(line, SPACES, 0);
			
			token = strtok(line+pos, space_delim);
					
			if(token == NULL)
				continue;
			
			if(strcmp(token,"macro")==0)
			{
				while(fgets(line,sizeof(line),fp1)!=NULL)
	    			{ 
					
					pos = skipper(line, SPACES, 0);
					token = strtok(line+pos, space_delim);
					if(token==NULL)
						continue;
					if(strcmp(token,"endmacro")==0)
						break;
	
					
				
				}
				fputc('\n',fp2);
				
			}
					
		
			else /*not a macro definition*/
			{
				
				np=lookup(token, macro_table);
				if(np!=NULL)
				{ 
					
					/*check if no extra text after macro call*/
					if(!check_end_command(token))
					{
						fputs(np->defn,fp2);
						fputc('\n', fp2);
					}
					else fputs(line2,fp2); /*else - copy line*/
				}
				else
				{
					fputs(line2,fp2);
				}
			}

		
		}/* end of while*/
		
	} /*end of else*/

	fclose(fp1);
	fclose(fp2);
}



