#include <stdio.h>
#include  <stdbool.h>
#include <stdlib.h>
#include "stack_frame.h"
#include <string.h>

stackFrame_t* global_stack_frame;

void stack_frame_create(stackFrame_t ** stack_frame)
{
    if (stack_frame != NULL)
    {
        (*stack_frame)=(stackFrame_t*)malloc(sizeof(stackFrame_t));
    }
}


void stack_frame_init(stackFrame_t* stack_frame)
{
    if (stack_frame != NULL)
    {
        stack_frame->top=-1;
    }
}

bool stack_frame_empty(stackFrame_t* stack_frame)
{
    return (stack_frame->top <= -1);
}

bool stack_frame_full(stackFrame_t* stack_frame)
{
    return (stack_frame->top == STACK_FRAME_SIZE -1);
}

void stack_frame_top(stackFrame_t* stack_frame, int *top)
{
    if (!stack_frame_empty(stack_frame))
    {
        *top=stack_frame->top;
    }
}

void stack_frame_pop(stackFrame_t* stack_frame)
{
    if (!stack_frame_empty(stack_frame))
    {
        frame_free(stack_frame->frame[stack_frame->top]->var_array,stack_frame->frame[stack_frame->top]->max);
        free(stack_frame->frame[stack_frame->top]->var_array);
        free(stack_frame->frame[stack_frame->top]);
        stack_frame->frame[stack_frame->top]=NULL;
        stack_frame->top--;
    }
}


void stack_frame_push(stackFrame_t* stack_frame,symData* function) //create frame
{
    if (!stack_frame_full(stack_frame))
    {
        stack_frame->top++;
        stack_frame->frame[stack_frame->top]=(frame_data_t*)malloc(sizeof(frame_data_t));
        stack_frame->frame[stack_frame->top]->var_array=(symData*)malloc(sizeof(symData)* function->funcdata_union.var_count);
        stack_frame->frame[stack_frame->top]->max=function->funcdata_union.var_count;
        stack_frame->frame[stack_frame->top]->args_counter=0;
        stack_frame->frame[stack_frame->top]->return_type=function->type;
    }
    else{
        printf("STACK FULL");
        exit(1);
    }
}

symData* set_return_value(stackFrame_t *stack_frame, symData* data)
{
    if (data == NULL)
    {
        return NULL;
    }
    else if (data->funcdata_union.offset < 0)
    {
        return data;
    }
    else
    {
        symData* tmp = pre_decode_addres(stack_frame,data->funcdata_union.offset);
        tmp->type=data->type;
        return tmp;
    }
}


void frame_push_variable(stackFrame_t* stack_frame,symData* variable)
{
    if (variable->type == tInt)
    {
        stack_frame->frame[stack_frame->top]->var_array[variable->funcdata_union.offset].ptr_union.i=0;
    }
    else if (variable->type == tDouble)
    {
        stack_frame->frame[stack_frame->top]->var_array[variable->funcdata_union.offset].ptr_union.d=0.0;
    }
    else if (variable->type == tString)
    {
        stack_frame->frame[stack_frame->top]->var_array[variable->funcdata_union.offset].ptr_union.str=NULL;
    }
}


symData* decode_addres(stackFrame_t* stack_frame, int offset)
{

    if (stack_frame->frame[stack_frame->top]->max > offset)
    {
        return &(stack_frame->frame[stack_frame->top]->var_array[offset]);
    }
    // NULL must be
    return NULL;
}

symData* pre_decode_addres(stackFrame_t *stack_frame, int offset)
{
    if (stack_frame->frame[stack_frame->top-1]->max > offset)
    {
        return &(stack_frame->frame[stack_frame->top-1]->var_array[offset]);
    }
    // NULL must be
    return NULL;
}

void Print_addres(symData* addres)
{
    if (addres->type == tInt)
    {
        printf("=> %d\n", addres->ptr_union.i);
    }
    else if (addres->type == tDouble)
    {
        printf("=> %f\n", addres->ptr_union.d);
    }
    else if (addres->type == tString)
    {
        printf("=> %s\n", addres->ptr_union.str);
    }
}


void frame_fill_variable(stackFrame_t* stack_frame,symData* variable)
{
    if (variable->type == tInt)
    {
        stack_frame->frame[stack_frame->top]->var_array[variable->funcdata_union.offset].ptr_union.i=variable->ptr_union.i;
    }
    else if (variable->type == tDouble)
    {
        stack_frame->frame[stack_frame->top]->var_array[variable->funcdata_union.offset].ptr_union.d=variable->ptr_union.d;
    }
    else if (variable->type == tString)
    {
        stack_frame->frame[stack_frame->top]->var_array[variable->funcdata_union.offset].ptr_union.str=(char*)malloc(sizeof(char) * ((int)strlen(variable->ptr_union.str)+1));
        strcpy(stack_frame->frame[stack_frame->top]->var_array[variable->funcdata_union.offset].ptr_union.str,variable->ptr_union.str);
    }

    stack_frame->frame[stack_frame->top]->var_array[variable->funcdata_union.offset].type=variable->type;
}

void arg_push(stackFrame_t *stack_frame,symData *data)
{
    symData *parameter;

    if(data->funcdata_union.offset < 0){
        parameter=data;
    }
    else{
        parameter=pre_decode_addres(stack_frame,data->funcdata_union.offset);

    }


    //Print_addres(parameter);

    if (data->type == tString)
    {
        stack_frame->frame[stack_frame->top]->var_array[stack_frame->frame[stack_frame->top]->args_counter].ptr_union.str=(char*)malloc(sizeof(char)*((int)strlen(parameter->ptr_union.str)+1));
        strcpy(stack_frame->frame[stack_frame->top]->var_array[stack_frame->frame[stack_frame->top]->args_counter].ptr_union.str,parameter->ptr_union.str);
        //printf("Ulozim na %d\n",stack_frame->frame[stack_frame->top]->args_counter);
    }
    else if (data->type == tInt)
    {
        stack_frame->frame[stack_frame->top]->var_array[stack_frame->frame[stack_frame->top]->args_counter].ptr_union.i=parameter->ptr_union.i;
        //printf("Ulozim na %d\n",stack_frame->frame[stack_frame->top]->args_counter);
    }
    else if (data->type == tDouble)
    {
        stack_frame->frame[stack_frame->top]->var_array[stack_frame->frame[stack_frame->top]->args_counter].ptr_union.d=parameter->ptr_union.d;
        //printf("Ulozim na %d\n",stack_frame->frame[stack_frame->top]->args_counter);
    }
    stack_frame->frame[stack_frame->top]->var_array[stack_frame->frame[stack_frame->top]->args_counter].init=true;
    stack_frame->frame[stack_frame->top]->var_array[stack_frame->frame[stack_frame->top]->args_counter].type=parameter->type;
    stack_frame->frame[stack_frame->top]->args_counter++;
}

void frame_free(symData* data_array,int max)
{
    for (int i=0; i<max;i++)
    {
        if (data_array[i].type == tString)
        {
            free(data_array[i].ptr_union.str);
        }
        else if (data_array[i].type == tInt)
        {
            data_array[i].ptr_union.i=0;
        }
        else if (data_array[i].type == tDouble)
        {
            data_array[i].ptr_union.d=0.0;
        }
    }
}

void Print_stack(stackFrame_t* stack_frame)
{
    for (int i=0;i<=stack_frame->top;++i)
    {
        printf("Frame %d: ",i);
        for (int j=0;j<stack_frame->frame[i]->max;++j)
        {
            if (stack_frame->frame[i]->var_array[j].type == tInt)
            {
                printf("offset %d : %d ",j,stack_frame->frame[i]->var_array[j].ptr_union.i);
            }
            else if (stack_frame->frame[i]->var_array[j].type == tDouble)
            {
                printf("offset %d : %f ",j,stack_frame->frame[i]->var_array[j].ptr_union.d);
            }
            else if (stack_frame->frame[i]->var_array[j].type == tString)
            {
                if (stack_frame->frame[i]->var_array[j].ptr_union.str != NULL)
                    printf("offset %d : %s ",j,stack_frame->frame[i]->var_array[j].ptr_union.str);
                else
                    printf("offset %d : NULL ",j);
            }
        }

        printf("\n");
    }
    printf("STACK EMPTY\n");
}

