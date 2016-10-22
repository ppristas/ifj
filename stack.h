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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//data nahradit za typ ktory chceme davat do stacku da
typedef struct SItem{

	struct SItem* nextptr;
	int data;

} tStackItem;


//rulz treba nahradit poriadnym enumom pravdepodobne
typedef struct{

	tStackItem *top;

}tStack;


void stackInit(tStack* s);
void stackPush(tStack* s, int dat);
bool stackEmpty(tStack* s);
void stackPop(tStack* s);


int stackTop(tStack* s);		//navratovy typ zmenit podla data ;
void stackPrint(tStack* s);
void stackFree(tStack* s);
