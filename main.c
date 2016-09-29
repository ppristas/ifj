#include <stdio.h>
#include <stdlib.h>
#include "scaner.h"
#include <stdbool.h>


bool arguments( int argc, char *argv[]){
	
	if(argc != 2){
		printf("Nespravny pocet argumentov\n");
		return false;
	}
	if( (file = fopen(argv[1],"r")) == NULL){
		printf("Chybny otvaraci subor\n");
		return false;
	}
	return true;
}


int main(int argc, char *argv[])
{
	if( !(arguments(argc, argv)) )
	{
		printf("Chyba pri spracovani argumentov\n");
		return 99;
	}
	get_token();
	if( (fclose(file)) == EOF){
		return 99;
	}	
	return 0;

}
