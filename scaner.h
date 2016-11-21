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

#ifndef SCANER_H
#define SCANER_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "error.h"
//typedef FILE *tfile;

//tfile file;

FILE *file;
typedef enum{
	S_START,			// 0 start
	S_END,				// 1 koncovy stav
	S_ERROR,			// 2 chyba
	S_ID,				// 3 jednoduchy identifikato
	S_CLASS,			// 4 zlozeny identifikator
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
	S_LZAT,				// 27 lava zatvorka
	S_PZAT,				// 28 prava zatvorka
	S_P_KOSZ,			// 29 prava kosata zatvorka
	S_L_KOSZ,			// 30 lava kosata zatvorka
	S_VYKR,				// 31 vykricnik
	S_NEROV,			// 32 != nerovna sa
	S_STRING,			//string "gfdsgfg"
	S_ESCAPE,			// escape sekvencia /n /t /ddd
	S_POMCLASS,
		
}TStav;

//structure of token
typedef struct{
    char *data;
    TStav stav;	
	int column;
	int line;
}Ttoken;

//structure of queue element
typedef struct QElem{
	Ttoken node;
	struct QElem *nextptr;
}tQuElem;

//structure of queue
typedef struct Queue{
	tQuElem *Front;
	tQuElem *Back;
}tQueue;

extern tQueue Queue_tok;
extern Ttoken token2;  
extern Ttoken token;

void init_Queue();
void front_token();
Ttoken get_token();

#endif
