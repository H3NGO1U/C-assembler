#include "text_func.h"
/*connect the text name to .xxx string*/
char* connect(const char *str1, const char *str2)
{
	char *connecting = malloc(strlen(str1)+strlen(str2)+1);
	if(connecting==NULL)
	{
		printf("Error: could not allocate memory!\n");
		return NULL;
	}
	if(connecting!=NULL)
	{
		strcpy(connecting, str1);
		strcat(connecting, str2);
		return connecting;
	}
	else
	{
		return NULL;
	}
}

char new_base[32]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

void decimalToSymbol(int num, char (*symbol)[SYMBOL_SIZE])
	{   
		int cnt=0, sy=0, index=1, i=0;
		char bin[WORD_SIZE+1];
	
		decimalToBinary(num,&bin);
		for (i=9;i>-1;i--)
		{
			sy+=pow(2,cnt)*(bin[i]-'0');
			cnt++;
/*evrey 5 bits is 1 symbol*/
		if(cnt==5)
		{
			(*symbol)[index]=new_base[sy];
			index--;
			sy=0;
			cnt=0;
			
		}
	}
	(*symbol)[2]='\0';
	    
}

int binaryToDecimal(char bin[WORD_SIZE+1]){
	int i,res=0,num=0;
/* char bin is a char '1' or '0'*/
	for (i=9;i>-1;i--)
	{
		res=pow(2,9-i)*(bin[i]-'0');
		num=res+num;
	}
	if(bin[0]=='1')
		return 0-(pow(2,10)-num);
	return num;
}


void decimalToBinary(int num, char (*bin)[WORD_SIZE+1]){
	unsigned int i=0,p=num;
	int j=0;
	char tmp;

	while(i<10)
	{
		(*bin)[i]= p%2+'0';
		p=p/2;
		i++;
	}
	for (j=0;j<5;j++)
	{
		tmp = (*bin)[9 - j];
		(*bin)[9 - j] = (*bin)[j];
		(*bin)[j] = tmp;
	}    
	(*bin)[10] = '\0';

}


void binaryToSymbol(char bin[WORD_SIZE], char (*symbol)[SYMBOL_SIZE]){
	int res;
	res=binaryToDecimal(bin);
	decimalToSymbol(res,symbol);

}

void reverseString(char value[], char (*reversed)[])
{
	int i, j = 0;
	for(i = WORD_SIZE-1; i>=0; i--) /*from right to left*/
	{
		(*reversed)[i] = value[j];
		j++;
	}
}


