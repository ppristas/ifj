/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          string.h                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"
#include "cleaner.h"


//Globálna páska na inštrukcie týkajúce sa inštrukcií globálnych premenných
ilist globalList;
//Ukazatel na momentalnu pasku
ilist * currentList;

//Generovanie inštrukcií na koniec danej inštrukčnej pásky

item * generateItem(eInstrType type, void *op1, void *op2, void *dest){
    instr *tmpInstruction;

    tmpInstruction = mymalloc(sizeof(instr));

    if(tmpInstruction == NULL){
        return NULL;
    }

    tmpInstruction->op1=op1;
    tmpInstruction->op2=op2;
    tmpInstruction->dest=dest;
    tmpInstruction->instrType=type;

    item* tmpItem;

    tmpItem = (item *)mymalloc(sizeof(item));
    if(tmpItem == NULL){
        return NULL;
    }

    tmpItem->instrPtr = tmpInstruction;
    tmpItem->prevItem = NULL;
    tmpItem->nextItem = NULL;

    return tmpItem;
}

//Vkladanie Navesti
void insertItem(item * I, ilist * L){
    if(I != NULL && L != NULL){
        if(L->last == NULL){
            L->last=I;
            L->first=I;
            I->prevItem=NULL;
            I->prevItem=NULL;
        }
        else{
            L->last->nextItem=I;
            I->prevItem=L->last;
            L->last=I;
            I->nextItem=NULL;
        }
    }
}
//Vlozi instrukciu na koniec momentalneho zoznamu
void generateLastInstruction(eInstrType type, void *op1, void *op2, void *dest, ilist *L)
{
    instr *tmp;

    tmp = (instr *)mymalloc(sizeof(instr));

    if(tmp == NULL){
        return;
    }

    tmp->op1=op1;
    tmp->op2=op2;
    tmp->dest=dest;
    tmp->instrType=type;

    insertLastInstruction(L, tmp);
}


//Inicializacia Listu
void listInit(ilist *L)
{
    L->active=NULL;
    L->first=NULL;
    L->last=NULL;
}

//Nastavenie aktívneho prvku na prvý prvok
void actFirst(ilist *L)
{
    if(L->first != NULL){
        L->active = L->first;
    }
}

//Nastavenie aktívneho prvku na posledný prvok
void actLast(ilist *L){
    if(L->last != NULL){
        L->active = L->last;
    }
}
/*
//Nastavenie aktívneho prvku na predchádzajúci prvok
void prevInstruction(ilist *L){
    if(L->active != NULL){
        L->active = L->active->prevItem;
    }
}
*/

//Iterácia aktívneho prvku o jedno dopredu
void succ(ilist *L)
{
    L->active = L->active->nextItem;
}

//Toto nie je nič pre vás
void insertLastInstruction(ilist *L, instr *I)
{
    item* tmp;

    tmp = (item *)mymalloc(sizeof(item));
    if(tmp == NULL){
        return;
    }

    if(L->first==NULL && L->last==NULL){
        tmp->instrPtr = I;
        tmp->prevItem = NULL;
        tmp->nextItem = NULL;
        L->first = tmp;
        L->last = tmp;
    }
    else{
        tmp->instrPtr = I;
        tmp->prevItem = L->last;
        tmp->nextItem = NULL;
        L->last->nextItem = tmp;
        L->last = tmp;
    }
}

//Uvoľní sa daná páska
void destroyList(ilist *L)
{
    item* tmp;

    while(L->first != NULL){
        tmp=L->first;
        L->first = L->first->nextItem;
        free(tmp->instrPtr);
        free(tmp);
    }
}

//Nič pre vás, získanie inštrukcie z aktívneho prvku
instr *getInstruction(ilist *L)
{
    return L->active->instrPtr;

}

//Získa sa posledný prvok Inštrukčnej pásky
item *getLast(ilist *L){
    return L->last;
}

//Inštrukcia na skoky
void setInstruction(ilist *L, item *instruction){
    L->active = instruction;
}



//Inicializacia stacku na odkladanie pasok
void stackInitList(instrStack *stack){
    stack->top=NULL;
    stack->pocet=0;
}
//Pushne poslednu pasku nastack
void stackPushList(instrStack *stack, item * instrPtr, ilist * L){
    stackItem * tmp;

    tmp = malloc(sizeof(struct stackItem));

    if(stack->top == NULL){
        tmp->instrPtr=instrPtr;
        tmp->list=L;
        tmp->nextItem=NULL;
        stack->top=tmp;
    }
    else{
        tmp->instrPtr=instrPtr;
        tmp->list=L;
        tmp->nextItem=stack->top;
        stack->top = tmp;
    }
    stack->pocet++;
}
//vypopne poslednu pasku
void stackPopList(instrStack *stack){
    stackItem * tmp;
    if(stack->top != NULL){
        stack->pocet--;
        tmp = stack->top;
        stack->top=stack->top->nextItem;
        free(tmp);
    }
}
//ziska poslednu odlozenu instrukciu
item *stackTopInstruction(instrStack * stack){
    if(stack->top != NULL){
     return stack->top->instrPtr;
    }

    else{
        return NULL;
    }
}
//ziska poslednu odlozenu pasku
ilist * stackTopList(instrStack * stack){
    if(stack->top != NULL){
     return stack->top->list;
    }

    else{
        return NULL;
    }
}
//zrusi stack na odkladanie pasok
void stackDestroyList(instrStack *stack){
    stackItem * tmp;
    while(stack->top != NULL){
        tmp = stack->top;
        stack->top= stack->top->nextItem;
        free(tmp);
    }
}
