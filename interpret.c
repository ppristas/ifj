#include <stdio.h>
#include "interpret.h"
#include "ilist.h"
#include <stdlib.h>


void interpret(ilist *L){
    actFirst(L);
    instr *I;
    symData *op1;
    symData *op2;
    symData *dest;

    while(L->active != NULL){
        I = getInstruction(L);


        switch (I->instrType) {
            case I_ADD:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;
                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tInt;
                    dest->ptr_union.i = op1->ptr_union.i + op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d + op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.i + op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d + op2->ptr_union.d;
                }
            break;

            case I_SUB:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;
                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tInt;
                    dest->ptr_union.i = op1->ptr_union.i - op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d - op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.i - op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d - op2->ptr_union.d;
                }
            break;

            case I_MUL:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;
                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tInt;
                    dest->ptr_union.i = op1->ptr_union.i * op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d * op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.i * op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d * op2->ptr_union.d;
                }


            break;

            case I_DIV:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;
                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tInt;
                    dest->ptr_union.i = op1->ptr_union.i / op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d / op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.i / op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tDouble;
                    dest->ptr_union.d = op1->ptr_union.d / op2->ptr_union.d;
                }

            break;


            case I_LESSEQ:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;
                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i <= op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d <= op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i <= op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d <= op2->ptr_union.d;
                }

            break;

            case I_LESS:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;
                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i < op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d < op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i < op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d < op2->ptr_union.d;
                }


            break;

            case I_BIGGER:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;

                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i > op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d > op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i > op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d > op2->ptr_union.d;
                }

            break;

            case I_BIGGEREQ:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;

                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i >= op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d >= op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i >= op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d >= op2->ptr_union.d;
                }

            break;

            case I_NOT:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;
                if(op1->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i;
                }
                if(op1->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d;
                }

            break;

            case I_EQ:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;
                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i == op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d == op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i == op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d == op2->ptr_union.d;
                }

            break;


            case I_NOTEQ:
                op1=I->op1;
                op2=I->op2;
                dest=I->dest;

                if(op1->type == tInt && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i != op2->ptr_union.i;
                }
                if(op1->type == tDouble && op2->type == tInt){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d != op2->ptr_union.i;
                }
                if(op1->type == tInt && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.i != op2->ptr_union.d;
                }

                if(op1->type == tDouble && op2->type == tDouble){
                    dest->type = tBool;
                    dest->ptr_union.is_bool = op1->ptr_union.d != op2->ptr_union.d;
                }


            break;

            case I_LABEL:
            break;

            case I_JMP:
                setInstruction(L, (item *)I->op1);
            break;

            case I_IFJMP:
                op2=I->op2;
                if(!op2->ptr_union.i){
                    setInstruction(L,I->op1);
                }








        }






        succ(L);
    }


}

