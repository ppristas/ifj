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
	//Ttoken test;
	if( !(arguments(argc, argv)) )
	{
		printf("Chyba pri spracovani argumentov\n");
		return 1;
	}
//	get_token();
	/** ULOZENIE DAT DO INEJ PREMENNEJ NESTACI LEN PRIRADIT

	
	test.data = (char*)malloc(strlen(token.data)*sizeof(char) +2);
	strcpy(test.data,token.data);
	test.stav = token.stav;	
	printf("predosly token: <%s> | stav = %d\n",test.data,test.stav); 

	free(test.data);
	*********************************/
	while(token.stav != S_EOF){
		get_token();
		printf("\nvrateny token:  |%s| | stav = %d error = %d\n\n",token.data,token.stav,error);
	}


	if(error == E_OK)
		printf("-----error: E_OK\n");		

//	printf("vrateny token:  <%s> | stav = %d\n",token.data,token.stav);	
	//printf("predosly token: <%s> | stav = %d\n",test.data,test.stav);
	free(token.data);
	token.data = NULL;
	
	if( (fclose(file)) == EOF){
		return 1;
	}	
	return error;

}

