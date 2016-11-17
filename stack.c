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

void stackInit(tStack* s){
	
	s->top = NULL;
}

bool stackEmpty(tStack* s){
	return(s->top == NULL);
}


void stackPush(tStack *s, SAData *data){
	tStackItem *pom;
	pom = mymalloc(sizeof(struct Sitem));	
	if( pom == NULL){
		error = INTERNAL_ERR;
		return;
	}
	pom->nextptr = s->top;
	pom->item = (*data);
	s->top = pom;
}


void stackPop(tStack *s){
	
	if(!stackEmpty(s)){
			s->top = s->top->nextptr;
			
	}	
}


void stackTop(tStack *s, SAData *send){
	if(!stackEmpty(s)){
		error = SUCCESS;
		(*send) = s->top->item;
	}
	
}


void stackTopPop(tStack *s, SAData *send){
	if(!(stackEmpty(s))){
		stackTop(s, send);
		stackPop(s);
	}

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
	
		stackPop(s);
	}
}
