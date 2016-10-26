/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          symbol.h                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/
#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include "scaner.h"
#include "ial.h"
//#include "str.h"

typedef enum {
   variable_integer,
   variable_boolean,
   variable_double,
   variable_string,
   variable_char,
   variable_array,//TODO IF NEEDED MORE VARIABLE TYPES
} variableType;

typedef enum {
   TString* name;//-----name of function i guess
   variableType type;
} functionArgs;

typedef struct TsymbolVariable {
   int initialized;        //if variable was initialized
   variableType type;      //type of variable
   union {                 //value of variable
      int value_integer;
      int value_boolean;
      double value_double;
      char value_char[1];
      char value_string[256];
   } value;
   char name[];            //name of variable
} symbolVariable;

typedef struct TsymbolFunction {
   TString* name;//--------------name of function
   variableType returnType;   //which type is function returning
   int defined;               //if function was defined
   unsigned int args_count;   //count of arguements
   functionArgs *args;
   /*TList * ins;*/
   /*HTab_t local_symbol;*/
} symbolFunction;

typedef struct TsymbolClass {
   TString* name;//--------------name of class
   struct symbolFunction *ptrFunc;
} symbolClass;

//Functions to be used ---TODO
void symbol_init();
#endif
