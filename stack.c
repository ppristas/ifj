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

void stackInit(tStack* s){
	
	s->top = NULL;
}

bool stackEmpty(tStack* s){
	return(s->top == NULL);
}

void stackPush(tStack *s, int dat){
	tStackItem *pom;
	pom = malloc(sizeof(struct SItem));	
	if( pom == NULL){
		error = INTERNAL_ERR;
		return;
	}
	pom->nextptr = s->top;
	pom->data = dat;
	s->top = pom;
}

void stackPop(tStack *s){
	
	tStackItem *pom;
	if(!stackEmpty(s)){
			pom = s->top;
			s->top = s->top->nextptr;
			free(pom);
	}	
}

int stackTop(tStack *s){
	if(!stackEmpty(s)){
		return s->top->data;
	}
	error = INTERNAL_ERR;
	return error;
}

void stackFree(tStack *s){
	while(s->top != NULL){
		stackPop(s);
	}
}

void stackPrint(tStack* s){
		
	if( s->top == NULL){
		printf("Je inicializovany bez polozky\n");
	}
	while(s->top != NULL){
		printf("[ %d ]\n",s->top->data);
		stackPop(s);
	}
}
