#include <stdio.h>
#include <stdlib.h>


#include "ial.h"
#include "cleaner.h"
#ifndef TEMP_TAB
#define TEMP_TAB

#define TMP_HTAB_SIZE 64

typedef struct tmp_item
{
    char * key;
    symData* data;
    struct tmp_item* next;
}T_TMP_ITEM;

typedef T_TMP_ITEM* TMP_HTAB[TMP_HTAB_SIZE];

void tmp_htab_insert(TMP_HTAB* table,symbolType type,void* ptr,char* key,int offset);
T_TMP_ITEM* tmp_htab_search(TMP_HTAB* table,char* key);
symData* tmp_get_data(TMP_HTAB* table,char* key);
void tmp_htab_init(TMP_HTAB* table);


extern TMP_HTAB* tabulka;

#endif
