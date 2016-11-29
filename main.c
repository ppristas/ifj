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
//#include "scaner.h"
#include <stdbool.h>
#include "error.h"
//#include "stack.h"
//#include "cleaner.h"
#include "parser.h"
#include "preced.h"
#include "temp_tab.h"

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
		return true;
	}
	else{
		printf("Implicitne zadane argumenty\n");
		return false;
	}

}


int main(int argc, char *argv[])
{
	initCleaner();
	init_Queue(&Queue_tok);
	token.stav = SUCCESS;
	token2.stav = SUCCESS;
	//Ttoken test;
	if( !(arguments(argc, argv)) )
	{
		fprintf(stderr,"Chyba pri spracovani argumentov\n");
		free(Mem);
		free(filename);
		clearAll();
		if(file != NULL)
			if(fclose(file) == EOF)
				return INTERNAL_ERR;
		return INTERNAL_ERR;
	}
/*
	while(token.stav != S_EOF){
		get_token();
		printf(":%d:%d: vrateny token:  |%s| | stav = %d error = %d\n\n",token.line,token.column,token.data,token.stav,error);
	}
*/
/*
//front_token();
	while(token2.stav != S_EOF){
	front_token();
		printf("%s :%d:%d: vrateny token2:  |%s| | stav = %d error = %d\n\n",filename,token2.line,token2.column,token2.data,token2.stav,error);
	//front_token();
    }
*/
	parser();
	if(error == SUCCESS)
		printf("-----error: E_OK\n");


//	expresion_parser();

	clearAll();
  free(token.data);
  free(filename);
	free(Mem);
	if(filename == NULL)
  	if(fclose(file) == EOF){
      	return INTERNAL_ERR;
  	}
  errorFce();
	return error;

}
