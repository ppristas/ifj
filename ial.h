/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          ial.h                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/
#ifndef IAL_H
#define IAL_H

#include <stdio.h>
#include "symbol.h"
//#include "str.h"

typedef enum {
   type_variable,
   type_function,
   type_class
} item_type;

//list item of symbols
typedef struct HTab_listitem_sym {
   item_type type;
   char key[];//-------may be in str.h
   struct HTab_listitem_sym *ptrnext;
   union {
      struct TsymbolVariable *ptr_variable;
      struct TsymbolFunction *ptr_function;
      struct TsymbolClass *ptr_class;
   } ptr;
} HTab_listitem;

//Hash table
typedef struct HTab {
   unsigned HTab_size;
   struct HTab_listitem_sym *list[];
} HTab_t;

//Functions to be used ------TODO
void HTab_init();//--HTab_t* HTab_init();
unsigned int HTab_hashCode();
HTab_listitem* HTab_insert();
HTab_listitem* HTab_search();
void HTab_remove();
void HTab_clear();

#endif
