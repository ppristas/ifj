#include <stdio.h>
#include "interpret.h"
#include "ilist.h"
#include <stdlib.h>
#include <string.h>
#include "buildin.h"
#include "cleaner.h"
#include "scaner.h"
#include "error.h"




void getOperands(symData **op1, symData **op2, symData **op3){
    symbolType type;
    if(op1 != NULL){
        if((*op1) != NULL){
            if((*op1)->funcdata_union.offset>=0){
                type = (*op1)->type;
                (*op1)=decode_addres(global_stack_frame, (*op1)->funcdata_union.offset);
                (*op1)->type = type;
            }
        }
    }
    if(op2 != NULL){
        if((*op2) != NULL){
            if((*op2)->funcdata_union.offset>=0){
                type = (*op2)->type;
                (*op2)=decode_addres(global_stack_frame, (*op2)->funcdata_union.offset);
                (*op2)->type = type;
            }
        }
    }
    if(op3 != NULL){
        if((*op3) != NULL){
            if((*op3)->funcdata_union.offset>=0){
                type = (*op3)->type;
                (*op3)=decode_addres(global_stack_frame, (*op3)->funcdata_union.offset);
                (*op3)->type = type;
            }
        }
    }
}



void interpret(ilist *L){

    ilist * tmpList;
    tmpList = L;
    actFirst(tmpList);

    instrStack stack;
    stackInitList(&stack);
    int pom;

    instr *I;
    symData *op1;
    symData *op2;
    symData *dest;
    char * tmpString;



    char tmpDouble[30];
    char tmpInt[20];
    symbolType tmpType;




    while(tmpList != NULL){
        if(tmpList -> active == NULL){
            tmpList = stackTopList(&stack);
            if(tmpList != NULL){
                tmpList->active=stackTopInstruction(&stack);
                stack_frame_pop(global_stack_frame);
                stackPopList(&stack);
                continue;
            }
            else{
                stack_frame_pop(global_stack_frame);
                break;
            }
        }

        I = getInstruction(tmpList);

        switch (I->instrType){
        case I_ASSIGN:
            op1=I->op1;
            op2=NULL;
            dest=I->dest;
            getOperands(&op1, &op2, &dest);
            if(op1->type == tInt && dest->type == tInt){
                if(op1->init==true){
                    dest->ptr_union.i = op1->ptr_union.i;
                    dest->init=true;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }

            }
            else if(op1->type == tInt && dest->type == tDouble){
                if(op1->init == true){
                    dest->ptr_union.d = op1->ptr_union.i;
                    dest->init=true;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && dest->type == tInt){
                if(op1->init == true){
                    dest->ptr_union.i = op1->ptr_union.d;
                    dest->init=true;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && dest->type == tDouble){
                if(op1->init == true){
                    dest->ptr_union.d = op1->ptr_union.d;
                    dest->init=true;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tString && dest->type == tString){
                if(op1->init == true){
                    if(dest->funcdata_union.offset == -1){
                        dest->ptr_union.str = mymalloc(strlen(op1->ptr_union.str) * sizeof(char) + 1);
                    }
                    else{
                        if(dest->init == true){
                            free(dest->ptr_union.str);
                        }
                        if(dest->funcdata_union.offset == -1){
                            dest->ptr_union.str = mymalloc(strlen(op1->ptr_union.str) * sizeof(char) + 1);
                        }
                        else{
                            dest->ptr_union.str = malloc(strlen(op1->ptr_union.str) * sizeof(char) + 1);
                        }
                    }
                    if(dest->ptr_union.str == NULL){
                        stackDestroyList(&stack);
                        error = 99;
                        return;
                    }
                    strcpy(dest->ptr_union.str, op1->ptr_union.str);
                    dest->init=true;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;

        case I_ADD:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt ){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tInt;
                    dest->ptr_union.i = op1->ptr_union.i + op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt ){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d + op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.i + op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble ){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d + op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tString && op2->type == tString){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tString;
                    dest->ptr_union.str=mymalloc(sizeof(char) * (strlen(op1->ptr_union.str) + strlen(op2->ptr_union.str)) + 1);
                    if(dest->ptr_union.str == NULL){
                        stackDestroyList(&stack);
                        error = 99;
                        return;
                    }
                    strcpy(dest->ptr_union.str, op1->ptr_union.str);
                    strcat(dest->ptr_union.str, op2->ptr_union.str);
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tString && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tString;
                    sprintf(tmpInt, "%d", op2->ptr_union.i);
                    dest->ptr_union.str=mymalloc(sizeof(char) * (strlen(op1->ptr_union.str) + strlen(tmpInt)) + 1);
                    if(dest->ptr_union.str == NULL){
                        stackDestroyList(&stack);
                        error = 99;
                        return;
                    }
                    strcpy(dest->ptr_union.str, op1->ptr_union.str);
                    strcat(dest->ptr_union.str, tmpInt);
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tString ){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tString;
                    sprintf(tmpInt, "%d", op1->ptr_union.i);
                    dest->ptr_union.str=mymalloc(sizeof(char) * (strlen(tmpInt) + strlen(op2->ptr_union.str)) + 1);
                    if(dest->ptr_union.str == NULL){
                        stackDestroyList(&stack);
                        error = 99;
                        return;
                    }
                    strcpy(dest->ptr_union.str, tmpInt);
                    strcat(dest->ptr_union.str, op2->ptr_union.str);
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return ;
                }
            }
            else if(op1->type == tString && op2->type == tDouble ){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tString;
                    sprintf(tmpDouble, "%g", op2->ptr_union.d);
                    dest->ptr_union.str=mymalloc(sizeof(char) * (strlen(op1->ptr_union.str) + strlen(tmpDouble)) + 1);
                    if(dest->ptr_union.str == NULL){
                        stackDestroyList(&stack);
                        error = 99;
                        return ;
                    }
                    strcpy(dest->ptr_union.str, op1->ptr_union.str);
                    strcat(dest->ptr_union.str, tmpDouble);
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tString){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tString;
                    sprintf(tmpDouble, "%g", op1->ptr_union.d);
                    dest->ptr_union.str=mymalloc(sizeof(char) * (strlen(tmpDouble) + strlen(op2->ptr_union.str)) + 1);
                    if(dest->ptr_union.str == NULL){
                        stackDestroyList(&stack);
                        error = 99;
                        return;
                    }
                    strcpy(dest->ptr_union.str, tmpDouble);
                    strcat(dest->ptr_union.str, op2->ptr_union.str);
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;

        case I_SUB:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tInt;
                    dest->ptr_union.i = op1->ptr_union.i - op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d - op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.i - op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d - op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;

        case I_MUL:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tInt;
                    dest->ptr_union.i = op1->ptr_union.i * op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d * op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.i * op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d * op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;
        case I_DIV:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tInt;
                    if(op2->ptr_union.i == 0){
                        stackDestroyList(&stack);
                        error = 9;
                        return;
                    }
                    dest->ptr_union.i = op1->ptr_union.i / op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    if(op2->ptr_union.i == 0){
                        stackDestroyList(&stack);
                        error = 9;
                        return;
                    }
                    dest->ptr_union.d = op1->ptr_union.d / op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    if(op2->ptr_union.d == 0.0){
                        stackDestroyList(&stack);
                        error = 9;
                        return;
                    }
                    dest->ptr_union.d = op1->ptr_union.i / op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tDouble;
                    if(op2->ptr_union.d == 0.0){
                        stackDestroyList(&stack);
                        error = 9;
                        return;
                    }
                    dest->ptr_union.d = op1->ptr_union.d / op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;
        case I_LESSEQ:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i <= op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d <= (double)op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = (double)op1->ptr_union.i <= op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool= op1->ptr_union.d <= op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;
        case I_LESS:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i < op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d < (double)op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = (double) op1->ptr_union.i < op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d < op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;
        case I_BIGGER:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i > op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d > (double)op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = (double) op1->ptr_union.i > op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d > op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;

        case I_BIGGEREQ:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i >= op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d >= (double) op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = (double) op1->ptr_union.i >= op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d >= op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;
        case I_EQ:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i == op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d == (double)op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = (double)op1->ptr_union.i == op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d == op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;

        case I_NOTEQ:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->type == tInt && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i != op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tInt){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d != (double)op2->ptr_union.i;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tInt && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = (double)op1->ptr_union.i != op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else if(op1->type == tDouble && op2->type == tDouble){
                if(op1->init == true && op2->init == true){
                    dest->init=true;
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d != op2->ptr_union.d;
                }
                else{
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
            }
            else{
                stackDestroyList(&stack);
                error = 4;
                return;
            }
            break;

        case I_LABEL:
            break;

        case I_JMP:
            setInstruction(tmpList, I->op1);
            break;

        case I_IFJMP:
            op1=NULL;
            op2=I->op2;
            dest=NULL;
            getOperands(&op1, &op2, &dest);
            if(op2->type == tBool && op2->init==true){
                if(!op2->ptr_union.is_bool){
                    setInstruction(tmpList, I->op1);
                }
            }
            else if(op2->type == tInt && op2->init==true){
                if(!op2->ptr_union.i){
                    setInstruction(tmpList, I->op1);
                }
            }
            else if(op2->type == tDouble && op2->init==true){
                if(!op2->ptr_union.d){
                    setInstruction(tmpList, I->op1);
                }
            }
            break;

        case I_FRAME:
            op1=I->op1;
            stack_frame_push(global_stack_frame, op1);
            break;

        case I_PUSHPARAM:
            op1=I->op1;
            op2=NULL;
            dest=NULL;
            arg_push(global_stack_frame, op1);
            break;

        case I_CALL:
            op1 = I->op1;
            op2 = I->op2;
            dest = NULL;
            global_stack_frame->frame[global_stack_frame->top]->return_addres=set_return_value(global_stack_frame, op2);
            if(global_stack_frame->frame[global_stack_frame->top]->return_addres != NULL){
                if(global_stack_frame->frame[global_stack_frame->top]->return_addres->type == tString){
                    if(global_stack_frame->frame[global_stack_frame->top]->return_addres->init == true && global_stack_frame->frame[global_stack_frame->top]->return_addres->funcdata_union.offset == 0){
                        free(global_stack_frame->frame[global_stack_frame->top]->return_addres->ptr_union.str);
                    }
                }
                global_stack_frame->frame[global_stack_frame->top]->return_addres->init=false;
            }
            stackPushList(&stack, tmpList->active->nextItem, tmpList);
            tmpList=op1->instrPtr;
            if(tmpList->first==NULL){
                tmpList = stackTopList(&stack);
                tmpList->active = stackTopInstruction(&stack);
                stack_frame_pop(global_stack_frame);
                stackPopList(&stack);
                continue;
            }
            else{
                actFirst(tmpList);
                continue;
            }

        case I_RET:
            op1 = I->op1;
            op2 = NULL;
            dest = NULL;
            getOperands(&op1, &op2, &dest);

            if(op1 != NULL && global_stack_frame->frame[global_stack_frame->top]->return_addres != NULL){
                if(op1->init == false){
                    stackDestroyList(&stack);
                    error = 8;
                    return;
                }
                if(op1->type == tInt && global_stack_frame->frame[global_stack_frame->top]->return_addres->type == tInt){
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->ptr_union.i = op1->ptr_union.i;
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->init=true;
                }
                if(op1->type == tInt && global_stack_frame->frame[global_stack_frame->top]->return_addres->type == tDouble ){
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->ptr_union.d = op1->ptr_union.i;
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->init=true;
                }
                if(op1->type == tDouble && global_stack_frame->frame[global_stack_frame->top]->return_addres->type == tInt ){
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->ptr_union.i = op1->ptr_union.d;
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->init=true;
                }
                if(op1->type == tDouble && global_stack_frame->frame[global_stack_frame->top]->return_addres->type == tDouble ){
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->ptr_union.d = op1->ptr_union.d;
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->init=true;
                }
                if(op1->type == tString && global_stack_frame->frame[global_stack_frame->top]->return_addres->type == tString ){
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->ptr_union.str = malloc(sizeof(char) * strlen(op1->ptr_union.str) + 1);
                    if(global_stack_frame->frame[global_stack_frame->top]->return_addres->ptr_union.str == NULL){
                        stackDestroyList(&stack);
                        error = 99;
                        return;
                    }
                    strcpy(global_stack_frame->frame[global_stack_frame->top]->return_addres->ptr_union.str, op1->ptr_union.str);
                    global_stack_frame->frame[global_stack_frame->top]->return_addres->init = true;
                }
            }
            tmpList = stackTopList(&stack);
            if(tmpList != NULL){
                tmpList->active = stackTopInstruction(&stack);
            }
            stack_frame_pop(global_stack_frame);
            stackPopList(&stack);

            continue;
            break;

        case I_PRINT:
            op1=I->op1;
            op2=NULL;
            dest=NULL;
            getOperands(&op1, &op2, &dest);

            if(op1->init==false){
                stackDestroyList(&stack);
                error = 8;
                return;
            }
            if(op1->type == tInt){
                printf("%d", op1->ptr_union.i);
            }
            else if(op1->type == tDouble){
                printf("%g", op1->ptr_union.d);
            }
            else if(op1->type == tString){
                printf("%s", op1->ptr_union.str);
            }
            break;
        case I_READI:
            op1 = NULL;
            op2 = NULL;
            dest = I->dest;
            getOperands(&op1, &op2, &dest);
            pom = readInt(dest);
            if(pom == 7 || pom == 4) {
                stackDestroyList(&stack);
                error = pom;
                return;
            }
            break;
        case I_READD:
            dest = I->dest;
            op2 = NULL;
            op1 = NULL;
            getOperands(&op1,&op2,&dest);
            pom = readDouble(dest);
            if(pom == 7 || pom == 4) {
                stackDestroyList(&stack);
                error = pom;
                return;
            }
            break;
        case I_READS:
            dest = I->dest;
            op2=NULL;
            op1=NULL;
            getOperands(&op1,&op2,&dest);
            readString(dest);
            break;
        case I_STRLEN:
            op1=I->op1;
            op2=NULL;
            dest=I->dest;
            getOperands(&op1,&op2,&dest);
            if(op1->init==false){
                stackDestroyList(&stack);
                error = 8;
                return ;
            }
            if(dest != NULL){
                if(dest->type == tInt){
                    dest->ptr_union.i=strlen(op1->ptr_union.str);
                    dest->init=true;
                }
                else if(dest->type == tDouble){
                    dest->ptr_union.d=strlen(op1->ptr_union.str);
                    dest->init=true;
                }
                else{
                    stackDestroyList(&stack);
                    error = 4;
                    return;
                }
            }
            break;
        case I_STRSORT:
            op1=I->op1;
            op2=NULL;
            dest=I->dest;
            getOperands(&op1, &op2, &dest);
            if(dest != NULL){
                if(dest->type==tString){
                    if(op1->init==true){
                        tmpString = malloc(sizeof(char) * strlen(op1->ptr_union.str) + 2);
                        strcpy(tmpString, sort(op1->ptr_union.str));
                        if(dest->init == true){
                            free(dest->ptr_union.str);
                        }
                        dest->ptr_union.str = malloc(sizeof(char ) * strlen(tmpString) + 2);
                        strcpy(dest->ptr_union.str, tmpString);
                        free(tmpString);
                        dest->init=true;
                    }
                    else{
                        stackDestroyList(&stack);
                        error = 8;
                        return;
                    }
                }
                else{
                    stackDestroyList(&stack);
                    error = 4;
                    return;
                }
            }
            break;
        case I_STRFIND:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1, &op2, &dest);
            if(op1->init == false || op2->init == false){
                stackDestroyList(&stack);
                error = 8;
                return;
            }
            if(dest != NULL){
                if(op1->type == tString && op2->type==tString){
                    if(dest->type==tInt){
                        dest->init=true;
                        dest->ptr_union.i=find(op1->ptr_union.str, op2->ptr_union.str);
                    }
                    else if(dest->type==tDouble){
                        dest->init=true;
                        dest->ptr_union.d=find(op1->ptr_union.str, op2->ptr_union.str);
                    }
                    else{
                        stackDestroyList(&stack);
                        error = 4;
                        return;
                    }
                }
            }
            break;
        case I_STRCMP:
            op1=I->op1;
            op2=I->op2;
            dest=I->dest;
            getOperands(&op1, &op2, &dest);
            if(op1->init == false || op2->init == false){
                stackDestroyList(&stack);
                error = 8;
                return;
            }
            if(dest != NULL){
                if(op1->type == tString && op2->type==tString){
                    if(dest->type==tInt){
                        dest->init=true;
                        dest->ptr_union.i=strcmp(op1->ptr_union.str, op2->ptr_union.str);
                        if(dest->ptr_union.i < 0){
                            dest->ptr_union.i = -1;
                        }
                        else if(dest->ptr_union.i > 0){
                            dest->ptr_union.i = 1;
                        }
                    }
                    else if(dest->type==tDouble){
                        dest->init=true;
                        dest->ptr_union.d=strcmp(op1->ptr_union.str, op2->ptr_union.str);
                    }
                    else{
                        stackDestroyList(&stack);
                        error = 4;
                        return;
                    }
                }
            }
            break;
        case I_STRSUB:
            dest = I->dest;
            tmpString = NULL;
            if(dest != NULL){
                tmpType = dest->type;
                dest=pre_decode_addres(global_stack_frame, dest->funcdata_union.offset);
                dest->type = tmpType;
            }
            if(global_stack_frame->frame[global_stack_frame->top]->var_array[2].init == false || global_stack_frame->frame[global_stack_frame->top]->var_array[1].init == false || global_stack_frame->frame[global_stack_frame->top]->var_array[0].init == false){
                stackDestroyList(&stack);
                error = 8;
                return;
            }
            if(dest != NULL){
                tmpString = malloc(sizeof(char) * global_stack_frame->frame[global_stack_frame->top]->var_array[2].ptr_union.i + 1);
                if(tmpString == NULL){
                    stackDestroyList(&stack);
                    error = 99;
                    return;
                }
                strcpy(tmpString, substring(global_stack_frame->frame[global_stack_frame->top]->var_array[0].ptr_union.str, global_stack_frame->frame[global_stack_frame->top]->var_array[1].ptr_union.i, global_stack_frame->frame[global_stack_frame->top]->var_array[2].ptr_union.i));
                if(dest->init == true){
                    free(dest->ptr_union.str);
                }
                dest->ptr_union.str = malloc(sizeof(char) * (global_stack_frame->frame[global_stack_frame->top]->var_array[2].ptr_union.i) + 1);
                strcpy(dest->ptr_union.str, tmpString);
                if(tmpString != NULL){
                    free(tmpString);
                }
                dest->init=true;
            }

            stack_frame_pop(global_stack_frame);
            break;


        } // UZAVIERKA SWITCHu


        succ(tmpList);



























































    }//UZAVIERKA WHILE-U
    error = 0;
    return ;
}
