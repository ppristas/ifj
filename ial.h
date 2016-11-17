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


#define Hash_table_size 256
#include "scaner.h"
#include <stdbool.h>

////////////////////////////////////////////////////////////
///////////nova struktura tabulky symbolov//////////////////
////////////////////////////////////////////////////////////

typedef struct _TNode {
   struct Sym_item *symarg;
   struct _TNode *next;
} TNode;

typedef struct _TList {
   TNode *first;
   TNode *act;
} TList;

typedef enum{
	tNan,
	tInt,
	tDouble,
	tString,
}symbolType;

typedef struct Sym_item{
	char *name;			//treba naalokovat sizeof(char)*strlen(token.data) + 2)
	symbolType type;	//typ ---moze byt aj navratova hodnota
	char *data;			//data napr i = 50 v datach bude char "50";
	bool fce;			//true - je funkcia, false - je premenna	
	TList *args;		//argumenty je potreba robit jednosmerne viazany zoznam
	bool init;
	char *class_name;	//nazov triedy ,ktorej patri symbol
	//bool isstatic;		true - globalna, false - lokalna TODO
	struct Sym_item *nextptr;
}iSymbol;


typedef struct Hash_table{

	iSymbol *ptr;
}Hash_item;	

typedef Hash_item tHTable[Hash_table_size];


extern tHTable *Main_table;

//linked list
TList* linked_list_init();
void list_insert_first(TList *list, iSymbol *arg);
void list_insert_next(TList *list, iSymbol *arg);
//symbol
symbolType sym_type(Ttoken token);
void sym_copy_variable(iSymbol* ptrsym1, iSymbol* ptrsym2);
void function_add_args(iSymbol* funcsym, iSymbol* arg, int counter);
iSymbol* sym_variable_init(char *data, int stype, bool isinit, char *classname);
iSymbol* sym_function_init(char *data, int stype, char *classname);
//table
void Htab_insert(tHTable* tab, iSymbol* newsymbol,char *data);
tHTable* HTab_init();
unsigned hash_function(char *data);
iSymbol* Htab_search(tHTable *ST, char *id);


/**********************************************************/

char* heapsort(char *ptr,unsigned int n);
int boyer_moore(const char *str, const char *pattern);

#endif
