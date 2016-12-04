#ifndef STACK_FRAME_H
#define STACK_FRAME_H

#include "ial.h"

#define STACK_FRAME_SIZE 126

typedef struct
{

    int max;
    int args_counter;
    symbolType return_type;
    symData* return_addres;
    symData* var_array;
}frame_data_t;

typedef struct _stack_frame
{
    int top;
    frame_data_t* frame[STACK_FRAME_SIZE];
}stackFrame_t;


extern stackFrame_t* global_stack_frame;

void stack_frame_create(stackFrame_t ** stack_frame);
void stack_frame_init(stackFrame_t *stack_frame);
bool stack_frame_empty(stackFrame_t* stack_frame);
bool stack_frame_full(stackFrame_t* stack_frame);
void stack_frame_top(stackFrame_t* stack_frame, int *top);
void stack_frame_pop(stackFrame_t* stack_frame);
void stack_frame_push(stackFrame_t* stack_frame,symData* function);
void frame_push_variable(stackFrame_t* stack_frame,symData* variable);
void frame_free(symData* data_array,int max);
void frame_fill_variable(stackFrame_t* stack_frame,symData* variable);
symData* decode_addres(stackFrame_t* stack_frame, int offset);
symData* pre_decode_addres(stackFrame_t* stack_frame,int offset);
symData *set_return_value(stackFrame_t* stack_frame, symData *data);
void arg_push(stackFrame_t* stack_frame,symData* data);
void Print_stack(stackFrame_t* stack_frame);
void Print_addres(symData* addres);



#endif // STACK_FRAME_H