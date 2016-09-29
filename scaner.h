/********************************************************************************
   *	Predmet:	Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)	*
   *	Projekt:	Interpret zjednoduseneho jazyka Java SE			*
   *	Variant:	b/2/II							*
   *	Tim:		092							*
   *	Subor:		scaner.h 						*
   *	Riesitelia:	Filip Mik 	(xmikfi00@stud.fit.vutbr.cz)		*
   *			Peter Ziska	(xziska02@stud.fit.vutbr.cz) 		*
   *			Peter Pristas	(xprist05@stud.fit.vutbr.cz)		*
   *			Daniel Florek	(xflore02@stud.fit.vutbr.cz)		*
   *			Martin Grnac	(xgrnac00@stud.fit.vutbr.cz)		*
   *										*		
   ********************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef FILE *tfile;

tfile file;

typedef enum{
	S_START,	//start
//	S_END,		//koncovy stav
//	S_ERROR,	//chyba
	S_JID,		//jednoduchy identifikato
	S_ZID,		//zlozeny identifikator
	S_INT,		//integer
//	S_DOUBL,	//double s bodkou
//	S_EXP,		//exponent v double cisle
//	S_EXPO,		//exponent so znamienkom
//	S_EX,		//pokracovanie cislom exponenta priklad: e24485654
//	S_PLUS,		//scitanie +
//	S_MINUS,	//odcitanie -
//	S_KRAT,		//nasobenie *
	//delenie doplnit
//	S_MEN,		//mensi <
//	S_MENROV,	//mensi rovny <=
//	S_VAC,		//vacsi >
//	S_VACROV,	//vacsi rovny >=
//	S_DIV,		//delenie /
//	S_KOMR,		//Riadkovy komentar
//	S_KOMBOX,	//Blokovy komentar /*	
	

}TStav;

typedef struct{
        char *data;
        TStav stav;
}Ttoken;



void test();
void get_token();
