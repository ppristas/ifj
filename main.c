#include <stdio.h>
#include <stdlib.h>
#include "scaner.h"
#include <stdbool.h>


bool arguments( int argc, char *argv[]){
	
	if(argc == 1){
		printf("Nespravny pocet argumentov\n");
		return false;
	}
	else if(argc == 2){
		if( (file = fopen(argv[1],"r")) == NULL){
			printf("Chybny otvaraci subor\n");
			return false;
		}
		return true;
	}
	else{
		printf("Implicitne zadane argumenty\n");
		return false;
	}
		
}


int main(int argc, char *argv[])
{

	if( !(arguments(argc, argv)) )
	{
		printf("Chyba pri spracovani argumentov\n");
		return 1;
	}
	
	get_token();	

	get_token();
	if(token.error == E_OK)
		printf("cosijaaaak\n");		
	printf("vrateny token:  <%s> | stav = %d\n",token.data,token.stav);	
	free(token.data);
	token.data = NULL;
	
	if( (fclose(file)) == EOF){
		return 1;
	}	
	return 0;

}

