/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          main.c                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/
   
#include <stdio.h>
#include <stdlib.h>
#include "scaner.h"
#include <stdbool.h>
#include "error.h"

char *filename = NULL;

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
		int length = strlen(argv[1]);
	    	filename = (char *)malloc(length*sizeof(char)+2);
	    	if( filename == NULL)
	       	 	return false;
	
	   	 strcpy(filename,argv[1]);

		return true;
	}
	else{
		printf("Implicitne zadane argumenty\n");
		return false;
	}
		
}


int main(int argc, char *argv[])
{
	token.stav = SUCCESS;
	//Ttoken test;
	if( !(arguments(argc, argv)) )
	{
		fprintf(stderr,"Chyba pri spracovani argumentov\n");
		return INTERNAL_ERR;
	}

	
	while(token.stav != S_EOF){
		get_token();
		printf("%s :%d:%d: vrateny token:  |%s| | stav = %d error = %d\n\n",filename,token.line,token.column,token.data,token.stav,error);
	}


	if(error == SUCCESS)
		printf("-----error: E_OK\n");		

//	printf("vrateny token:  <%s> | stav = %d\n",token.data,token.stav);	
	//printf("predosly token: <%s> | stav = %d\n",test.data,test.stav);
	free(token.data);
	free(filename);
	token.data = NULL;
	
	if( (fclose(file)) == EOF){
		return 1;
	}	
	return error;

}

