/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          stack.h                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/
#include "preced.h"
#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
//#include "preced.h"		//obsahuje ial.h>obsahuje scaner.h

//data nahradit za typ ktory chceme davat do stacku da
typedef struct Sitem{

	struct Sitem* nextptr;
	SAData item;

} tStackItem;


//rulz treba nahradit poriadnym enumom pravdepodobne
typedef struct{

	tStackItem *top;

}tStack;


void stackInit(tStack* s);
void stackPush(tStack* s, SAData *data);
bool stackEmpty(tStack* s);
void stackPop(tStack* s);
void stackTopPop(tStack *s,SAData *data);

void stackTop(tStack* s, SAData *data);		//navratovy typ zmenit podla data ;
void stackPrint(tStack* s);
void stackFree(tStack* s);

#endif //STACK_H
