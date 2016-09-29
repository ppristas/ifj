/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          main.c                                               *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/

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
