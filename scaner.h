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
	S_EX,				// 11 pokracovanie cislom exponenta priklad: e24485654
	S_PLUS,				// 12 scitanie +
	S_MINUS,			// 13 odcitanie -
	S_MULTI,			// 14 nasobenie *
	S_DIV,				// 15 delenie//delenie doplnit
	S_COM_LINE,			// 16 riadkovy komentar 		
	S_MULTI_COM,		// 17 multi komentar
	S_EOF,				// 18 Koniec dokumentu
	S_SEMICOLON,		// 19 bodkociarka
	S_PRIR,				// 20 priradenie
	S_MEN,				// 21 mensi <
	S_MENROV,			// 22 mensi rovny <=
	S_VAC,				// 23 vacsi >
	S_VACROV,			// 24 vacsi rovny >=	
	S_ROVNY,			// 25 rovnost ==
	S_CIARKA,			// 26 ciarka
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
