/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          stack.c                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *
   ********************************************************************************/

#include "stack.h"
#include "error.h"
#include "cleaner.h"

/**
  * function: inicializuje zasobnik
	* params: s-> zasobnik ktory sa ma pouzit
	**/
void stackInit(tStack* s){

	s->top = NULL;
}

/**
  * function: zisti ci je zasobnik prazdny
	* params: s-> zasobnik ktory sa ma pouzit
	* return value: true -> ak je prazdny inak false
	**/
bool stackEmpty(tStack* s){
	return(s->top == NULL);
}

/**
  * function: vlozi na vrchol zasobnika polozku
	* params: s-> zasobnik ktory sa ma pouzit
	* params: data -> data ktore sa maju vlozit do zasobnika
	**/
void stackPush(tStack *s, SAData *data){
	tStackItem *pom;
	pom = mymalloc(sizeof(tStackItem));
	if( pom == NULL){
		error = INTERNAL_ERR;
		return;
	}
	pom->nextptr = s->top;
	pom->item = (*data);
	s->top = pom;
}

/**
  * function: uvolni polozku na vrchole zasobnika
	* params: s-> zasobnik ktory sa ma pouzit
	**/
void stackPop(tStack *s){

	if(!stackEmpty(s)){
			s->top = s->top->nextptr;

	}
}

/**
  * function: preda hodnotu z vrcholu zasobnika
	* params: s-> zasobnik ktory sa ma pouzit
	* params: send-> v tejto premennej sa preda hodnota z vrcholu zasonika
	**/
void stackTop(tStack *s, SAData *send){
	if(!stackEmpty(s)){
		error = SUCCESS;
		(*send) = s->top->item;
	}

}

/**
  * function: preda a da prec hodnotu z vrcholu zasobnika
	* params: s-> zasobnik ktory sa ma pouzit
	* params: send -> v tejto premennej sa preda hodnota z vrcholu zasonika
	**/
void stackTopPop(tStack *s, SAData *send){
	if(!(stackEmpty(s))){
		stackTop(s, send);
		stackPop(s);
	}

}

/**
  * function: uvolni zasobnik
	* params: s-> zasobnik ktory ma byt uvolneny
	**/
void stackFree(tStack *s){
	while(s->top != NULL){
		stackPop(s);
	}
}

/**
  * function: vypise polozky zasobniku
	* params: s-> zasobnik ktory ma byt vypisany
	**/
void stackPrint(tStack* s){

	if( s->top == NULL){
		printf("Je inicializovany bez polozky\n");
	}
	while(s->top != NULL){

		stackPop(s);
	}
}
