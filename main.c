#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "text_func.h"
#include "macro.h"
#include "hash.h"
#include "first_transition.h"

int main(int argc,char const *argv[])
{
	int i, general_error = 0, error;
	


	
	/*cheaking if the users write filename*/
	if(argc<=1)
	{
		printf("Error:No File\n");
		return 1;
	}
	/*open all the files in the programe*/
	for(i=1; i<argc; i++)
	{
	
		error = macro_to_table(argv[i]);
		if(!general_error) 
			general_error = error;

	}
	
	/*open the .am files only if there weren't mistakes in the macro stage*/
	for(i = 1; !general_error && i<argc; i++)
	{
		
		intrprtr(argv[i]);
		printf("\n-----------------------------------------------------------------------------------\n");
	}
	
	return 1;
}
	


