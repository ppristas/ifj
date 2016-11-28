#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

#include "ilist.h"
#include <stdbool.h>
void interpret(ilist *L);

typedef enum{
    tNan,
    tInt,
    tDouble,
    tString,
    tBool,
}symbolType;

typedef struct sym_Data {
    int arg_count;
    //TList *args;
    symbolType type;
    ilist* instrPtr;
    union {
        int i;
        double d;
        char *str;
        bool is_bool;
    }ptr_union;
} symData;

typedef struct Loc_item {
    symData* data;
    char *name;
    bool init;
    bool decl;
    char *class_name;
    struct Loc_item *nextptr;
}locSymbol;


typedef struct Sym_item{
    symData* data;
    char *name;						//treba naalokovat sizeof(char)*strlen(token.data) + 2)
    bool init;
    bool decl;
    //locTable *ptr_loctable; 	//tabulka pre lokalne symboly
    char *class_name;				//nazov triedy ,ktorej patri symbol
    bool isstatic;					//true - globalna, false - lokalna
    struct Sym_item *nextptr;
}iSymbol;


#endif // INTERPRET_H_INCLUDED
