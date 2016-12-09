#ifndef STACK_FRAME_H
#define STACK_FRAME_H

#include "ial.h"

#define STACK_FRAME_SIZE 126
//Ramec
typedef struct
{

    int max;
    int args_counter;
    symbolType return_type;
    symData* return_addres;
    symData* var_array;
}frame_data_t;
//Stack Frame
typedef struct _stack_frame
{
    int top;
    frame_data_t* frame[STACK_FRAME_SIZE];
}stackFrame_t;

//Inicializacia globalneho stack_frame
extern stackFrame_t* global_stack_frame;

//Vytvori sa stack pre ramce
void stack_frame_create(stackFrame_t ** stack_frame);
//Vycisti cely stack
void vycisti_mi_cely_stak(stackFrame_t *stack_frame);
//Nainicializuje stack
void stack_frame_init(stackFrame_t *stack_frame);
//Zisti, ci je stack prazdny
bool stack_frame_empty(stackFrame_t* stack_frame);
//Zisti sa, ci je stack plny
bool stack_frame_full(stackFrame_t* stack_frame);
//Prekopiruje hodnotu posledneho framu
void stack_frame_top(stackFrame_t* stack_frame, int *top);
//Zrusi posledny frame
void stack_frame_pop(stackFrame_t* stack_frame);
//Vytvori sa novy frame
void stack_frame_push(stackFrame_t* stack_frame,symData* function);
//void frame_push_variable(stackFrame_t* stack_frame,symData* variable);
//Pomocna funkcia na uvolnenie retazcov z framu
void frame_free(symData* data_array,int max);
//void frame_fill_variable(stackFrame_t* stack_frame,symData* variable);
//Sluzi na preklad adries
symData* decode_addres(stackFrame_t* stack_frame, int offset);
//Sluzi na dekodovanie adresy navratovej hodnoty
symData* pre_decode_addres(stackFrame_t* stack_frame,int offset);
//Nastavi navratovu hodnotu
symData *set_return_value(stackFrame_t* stack_frame, symData *data);
//Sluzi na pushovanie argumentov do ramca
void arg_push(stackFrame_t* stack_frame,symData* data);
//Pomocna funkcia na vypis
void Print_stack(stackFrame_t* stack_frame);
//Pomocna funkcia na vypis
void Print_addres(symData* addres);



#endif // STACK_FRAME_H
