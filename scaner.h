/********************************************************************************
   *	Predmet:	Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)			*
   *	Projekt:	Interpret zjednoduseneho jazyka Java SE						*
   *	Variant:	b/2/II														*
   *	Tim:		092															*
   *	Subor:		scaner.h 													*
   *	Riesitelia:	Filip Mik 	(xmikfi00@stud.fit.vutbr.cz)					*
   *			Peter Ziska		(xziska02@stud.fit.vutbr.cz) 					*
   *			Peter Pristas	(xprist05@stud.fit.vutbr.cz)					*
   *			Daniel Florek	(xflore02@stud.fit.vutbr.cz)					*
   *			Martin Grnac	(xgrnac00@stud.fit.vutbr.cz)					*
   *																			*		
   ******************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//typedef FILE *tfile;

//tfile file;

FILE *file;
typedef enum{
	S_START,	//start
	S_END,		//koncovy stav
	S_ERROR,	//chyba
	S_ID,		//jednoduchy identifikato
	S_MULTI_ID,		//zlozeny identifikator
	S_INT,		//integer
	S_DOUBLE,	//double s bodkou
	S_EXP,		//exponent v double cisle
//	S_EXPO,		//exponent so znamienkom
//	S_EX,		//pokracovanie cislom exponenta priklad: e24485654
//	S_PLUS,		//scitanie +
//	S_MINUS,	//odcitanie -
//	S_KRAT,		//nasobenie *
//	S_DIV,		//delenie//delenie doplnit

//	S_MEN,		//mensi <
//	S_MENROV,	//mensi rovny <=
//	S_VAC,		//vacsi >
//	S_VACROV,	//vacsi rovny >=
//	S_DIV,		//delenie /
//	S_KOMR,		//Riadkovy komentar
//	S_KOMBOX,	//Blokovy komentar /*	
//	S_CIARKA,	//ciarka
//	S_STREDNIK,	//bodkociarka ;

}TStav;


typedef enum{
	E_OK,
	E_LEXICAL,
	E_INTERNAL,
}EErrr;

typedef struct{
        char *data;
        TStav stav;
		EErrr error;
}Ttoken;


extern Ttoken token;

//bool test_reserved_words();i
Ttoken get_token();
