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
	S_START,			// 0 start
	S_END,				// 1 koncovy stav
	S_ERROR,			// 2 chyba
	S_ID,				// 3 jednoduchy identifikato
	S_MULTI_ID,			// 4 zlozeny identifikator
	S_KEY,				// 5 klucove slova
	S_INT,				// 6 integer
	S_DOUBLE_POM,		// 7 double s bodkou
	S_DOUBLE,			// 8 double pomocny
	S_EXP,				// 9 exponent v double cisle
	S_EXP_SIGNED,		// 10 exponent so znamienkom
//	S_EXP_SIGNED_POM,	// 11 pomocna pre exponent
	S_EX,				// 12 pokracovanie cislom exponenta priklad: e24485654
//	S_PLUS,				// 13 scitanie +
//	S_MINUS,			// 14 odcitanie -
//	S_KRAT,				// 15 nasobenie *
//	S_DIV,				// 16 delenie//delenie doplnit


//	S_MEN,		//mensi <
//	S_MENROV,	//mensi rovny <=
//	S_VAC,		//vacsi >
//	S_VACROV,	//vacsi rovny >=
//	S_DIV,		//delenie /
//	S_KOMR,		//Riadkovy komentar
//	S_MULTI_KOM,	//Blokovy komentar /*	
//	S_CIARKA,	//ciarka
//	S_STREDNIK,	//bodkociarka ;
//	S_STRING,	//string "
}TStav;


typedef enum{
	E_OK,
	E_LEXICAL,
	E_INTERNAL,
}Enum_error;

typedef struct{
        char *data;
        TStav stav;	
}Ttoken;

extern Enum_error error; 
extern Ttoken token;

//bool test_reserved_words();i
Ttoken get_token();
