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
#include "ial.h"
//#include "symbol.h"

int HTSIZE = MAX_HTSIZE;
HTab_table* ptrht;
//HTab_listitem_sym* UNDEFPTR;

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

void printhash(HTab_table* ptrht) {
	int maxlen = 0;
	int sumcnt = 0;

	printf("------------Hash biiitch----------\n");
	for(int i = 0; i < HTSIZE; i++) {
		printf("%i:",i);
		int cnt = 0;
		HTab_listitem_sym* ptritem = (*ptrht)[i];
		while(ptritem != NULL) {
			printf("(%s %d)",((*ptritem).token.data),ptritem->token.stav);
			ptritem = ptritem->ptrnext;
		}
		printf("\n");
		if(cnt > maxlen)
			maxlen = cnt;
		sumcnt += cnt;
	}
	printf("-----------------------------\n");
}

int main(int argc, char *argv[])
{
	//Ttoken test;
	if( !(arguments(argc, argv)) )
	{
		printf("Chyba pri spracovani argumentov\n");
		return 1;
	}
	//------------------------------

	ptrht = (HTab_table*) malloc(sizeof(HTab_table));
	HTSIZE = 19;
	printhash(ptrht);

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
		HTab_insert(ptrht,token);
	}
	printhash(ptrht);


	if(error == E_OK)
		printf("-----error: E_OK\n");		

//	printf("vrateny token:  <%s> | stav = %d\n",token.data,token.stav);	
	//printf("predosly token: <%s> | stav = %d\n",test.data,test.stav);
	free(token.data);
	token.data = NULL;
	
	//---------------------------------
	HTab_free(ptrht);
	if( (fclose(file)) == EOF){
		return 1;
	}	
	return error;

}

