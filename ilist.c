#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"


//Globálna páska na inštrukcie týkajúce sa inštrukcií globálnych premenných
ilist globalList;
ilist mainList;

//Generovanie inštrukcií na koniec danej inštrukčnej pásky
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

//Generovanie inštrukcií za aktívny prvok
void generatePostInstruction(eInstrType type, void *op1, void *op2, void *dest, ilist *L){
    instr *tmp;

    tmp = malloc(sizeof(instr));

    if(tmp == NULL){
        return;
    }

    tmp->op1=op1;
    tmp->op2=op2;
    tmp->dest=dest;
    tmp->instrType=type;

    insertPostIntruction(L, tmp);
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

//Nastavenie aktívneho prvku na predchádzajúci prvok
void prevInstruction(ilist *L){
    if(L->active != NULL){
        L->active = L->active->prevItem;
    }
}


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

//Toto nie je nič pre vás
void insertPostIntruction(ilist *L, instr *I){
    if(L->active == NULL){
        return;
    }

    item* tmp;

    tmp = malloc(sizeof(item));
    if(tmp == NULL){
        return;
    }

    if(L->active->nextItem != NULL){
        tmp->instrPtr = I;
        tmp->prevItem = L->active;
        tmp->nextItem = L->active->nextItem;
        tmp->nextItem->prevItem=tmp;
        L->active->nextItem = tmp;

    }

    if(L->active->nextItem == NULL){
        tmp->prevItem = L->active;
        tmp->nextItem = NULL;
        L->active->nextItem = tmp;
        tmp->instrPtr=I;
    }

    L->active = tmp;

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
