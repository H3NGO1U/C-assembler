
#include "hash.h"


unsigned hash(char *s)
{
	unsigned hashval;
	for(hashval=0;*s!='\0';s++)
		hashval=*s+31*hashval;		
	return hashval % HASHSIZE;
}

void createTable(nlist *(*hashtab)[HASHSIZE])
{
	int i = 0;
	for(i = 0; i<HASHSIZE; i++) 
	{
		(*hashtab)[i] = NULL;
		
		
	}
}
nlist *lookup(char *s, nlist *hashtab[HASHSIZE])
{
	nlist *np;

	
	for(np=hashtab[hash(s)]; np!=NULL; np=np->next)
		if(strcmp(s,np->name)==0)
			return np;
	
	return NULL;
	
}




int install(char *name,char *defn, nlist *(*hashtab)[HASHSIZE], int address)
{
	nlist *np;
	unsigned hashval;
	if(strlen(name)>=MAX_OPP) return TOO_LONG;
	
	if((np = lookup(name, *hashtab)) == NULL)
	{
		np = (nlist*)malloc(sizeof(nlist));
		strcpy(np->name, name);

		
		if((np)==NULL||(np->name)==NULL)
			return SERIOUS_ERR;
	
		(np)->address = address; 

		hashval= hash(name);
		
		
		((np)->next)=(*hashtab)[hashval];

		
		(*hashtab)[hashval]=(np);
		if((((np)->defn)=strdup(defn))==NULL)
			return SERIOUS_ERR;
	
	}	
	
	else 
	{
		return ALREADY_DEFINED;
	}

	
	
	return DEF;
	
}


void display(nlist *hashtab[HASHSIZE]) {
   	int i;
	nlist *np;
	if(hashtab == NULL)
		return;
	for(i = 0; i<HASHSIZE; i++) /*go through the table*/
	{
		np = hashtab[i];
		if(np!=NULL)
			while(np!=NULL) /*print all the elements*/
			{
				printf(" (%s %s %d) ",np->name,np->defn, np->address);
				np = np->next;	
			}
		else
			printf(" ~~ ");
		
	}
	
   
}

void destroy(nlist *(*hashtab)[HASHSIZE])
{
 	int i;
	nlist *np, *temp;
	for(i = 0; i<HASHSIZE; i++) 
	{
		np = (*hashtab)[i];
		while(np!=NULL)
		{
			temp = np;
			np = np->next;
			free(temp->defn);
			temp->defn = NULL;
			free(temp);
			temp = NULL;
		}
	}



	
}


