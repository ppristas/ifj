#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"


//Globálna páska na inštrukcie týkajúce sa inštrukcií globálnych premenných
ilist globalList;
ilist * currentList;
labelStack lStack;

//Generovanie inštrukcií na koniec danej inštrukčnej pásky

item * generateItem(eInstrType type, void *op1, void *op2, void *dest){
    instr *tmpInstruction;

    tmpInstruction = malloc(sizeof(instr));

    if(tmpInstruction == NULL){
        return NULL;
    }

    tmpInstruction->op1=op1;
    tmpInstruction->op2=op2;
    tmpInstruction->dest=dest;
    tmpInstruction->instrType=type;

    item* tmpItem;

    tmpItem = malloc(sizeof(item));
    if(tmpItem == NULL){
        return NULL;
    }

    tmpItem->instrPtr = tmpInstruction;
    tmpItem->prevItem = NULL;
    tmpItem->nextItem = NULL;

    return tmpItem;
}

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

void generateLastInstruction(eInstrType type, void *op1, void *op2, void *dest, ilist *L)
{
    instr *tmp;

    tmp = malloc(sizeof(instr));

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

    tmp = malloc(sizeof(item));
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




void stackInitList(instrStack *stack){
    stack->top=NULL;
    stack->pocet=0;
}

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

void stackPopList(instrStack *stack){
    stackItem * tmp;
    if(stack->top != NULL){
        stack->pocet--;
        tmp = stack->top;
        stack->top=stack->top->nextItem;
        free(tmp);
    }
}

item *stackTopInstruction(instrStack * stack){
    if(stack->top != NULL){
     return stack->top->instrPtr;
    }

    else{
        return NULL;
    }
}

ilist * stackTopList(instrStack * stack){
    if(stack->top != NULL){
     return stack->top->list;
    }

    else{
        return NULL;
    }
}

void stackDestroy(instrStack *stack){
    stackItem * tmp;
    while(stack->top != NULL){
        tmp = stack->top;
        stack->top= stack->top->nextItem;
        free(tmp);
    }
}

void labelStackInit(labelStack *stack){
    stack->top=NULL;
}

void labelStackPush(labelStack *stack, item *label1){
    labelItem * tmp;
    tmp=malloc(sizeof(labelItem));
    if(tmp == NULL){
        exit(99);
    }

    if(stack->top==NULL){
        tmp->label=label1;
        stack->top=tmp;
        tmp->nextItem=NULL;
    }

    if(stack->top != NULL){
        tmp->label = label1;
        tmp->nextItem=stack->top;
        stack->top=tmp;
    }
}

item * labelStackTop(labelStack * stack){
    if(stack->top != NULL){
        return stack->top->label;
    }
    return NULL;
}

item * labelStackPrevTop(labelStack * stack){
    if(stack->top != NULL){
        if(stack->top->nextItem != NULL){
            return stack->top->nextItem->label;
        }
    }
    return NULL;
}

void labelStackPop(labelStack * stack){
    labelItem * tmp;
    if(stack->top != NULL){
        tmp = stack->top;
        stack->top=stack->top->nextItem;
        free(tmp);
    }
    if(stack->top != NULL){
        tmp = stack->top;
        stack->top=stack->top->nextItem;
        free(tmp);
    }
}


