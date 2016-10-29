/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          ial.h                                                   *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/
#ifndef IAL_H
#define IAL_H

#include "scaner.h"
typedef struct HTab_listitem_sym {
   Ttoken token;
   struct HTab_listitem_sym* ptrnext;
} HTab_listitem;

typedef struct HTab_table {
	unsigned htable_size;
	struct HTab_listitem_sym *list[];
} HTab_t;

HTab_t* HTab_init(unsigned size);
HTab_listitem* HTab_insert(HTab_t* ptrht, Ttoken token);
unsigned hash_function(unsigned size, Ttoken token);
void HTab_free(HTab_t* ptrht);
void HTab_clear(HTab_t* ptrht);
void HTab_remove(HTab_t* ptrht,Ttoken token);

//HTab_listitem* HTab_insert(HTab_t* ptrht, Ttoken token);

/*typedef enum {
   type_variable,
   //type_function,
   //type_class
} item_type;

//list item of symbols
typedef struct HTab_listitem_sym {
   item_type type;
   char* key;//-------may be in str.h
   struct HTab_listitem_sym *ptrnext;
   union {
      struct TsymbolVariable *ptr_variable;
      //struct TsymbolFunction *ptr_function;
      //struct TsymbolClass *ptr_class;
   } ptr;
} HTab_listitem;

//Hash table
typedef struct HTab {
   unsigned HTab_size;
   struct HTab_listitem_sym *list[];
} HTab_t;

//Functions to be used ------TODO
HTab_t* HTab_init(HTab_t* ptrht);
unsigned int hash_function(HTab_t* ptrht,char* data)
void HTab_free(HTab_t* ptrht);*/

void heapsort(char *ptr, char *str_ret, unsigned int n);

int boyer_moore(const char *str, const char *pattern);

#endif
