/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          preced.h                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *
   ********************************************************************************/

//#include "scaner.h"
#include "ial.h"

#ifndef PRECED_H

#define PRECED_H

	//#include "ial.h"
int expresion_parser();

typedef enum Cikundex {
   PLUS,    //0 +
   MINUS,   //1 -
   KRAT,    //2 *
   DELENE,  //3 /
   LZATV,   //4 (
   PZATV,   //5 )
   ID,      //6 Konstanta alebo identifikator
   FUNC,    //7 Volanie funkcie
   MENSIE,  //8 <
   VACSIE,  //9 >
   MENROV,  //10 <=
   VACROV,  //11 >=
   EQUAL,   //12 ==
   NEQUAL,  //13 !=
   CARKA,   //14 ,
   DOLAR,   //15 Koniec vyrazu - $

   L_HANDLE,   //16 { zaciatok handlu
   R_HANDLE,   //17 } koniec handlu
   NETERM,     //18 neterminal
   ERROR,      //19 chyba
} tPrec_enum;


typedef struct{
	symData *sym_data;
  char* nameID;
	tPrec_enum indexibus;
}SAData;

extern symData *destExpr;
#endif //PRECED_H
