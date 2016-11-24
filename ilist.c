#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"

ilist instrList;

void generateInstruction(eInstrType type, void *op1, void *op2, void *dest)
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

    insertInstruction(&instrList, tmp);
}

void listInit(ilist *L)
{
    L->active=NULL;
    L->first=NULL;
    L->last=NULL;
}

void first(ilist *L)
{
    if(L->first != NULL){
        L->active = L->first;
    }
}

void succ(ilist *L)
{
    L->active = L->active->nextItem;
}

void insertInstruction(ilist *L, instr *I)
{
    item* tmp;

    tmp = malloc(sizeof(item));
    if(tmp == NULL){
        return;
    }

    if(L->first==NULL){
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

instr *getInstruction(ilist *L)
{
    return L->active->instrPtr;

}

item *getLast(ilist *L){
    return L->last;
}

void setInstruction(ilist *L, item * instruction){
    L->active=instruction;
}
