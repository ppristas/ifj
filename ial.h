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

typedef struct Loc_item {
	char *name;
	symbolType type;
	char *data;
	bool fce;
	TList *args;
	bool init;
	bool decl;
	char *class_name;
	struct Loc_item *nextptr;
}locSymbol;

typedef struct local_table{
	locSymbol *ptr;
}Hash_local_item;

typedef Hash_local_item locTable[Hash_table_size];

typedef struct Sym_item{
	char *name;						//treba naalokovat sizeof(char)*strlen(token.data) + 2)
	symbolType type;				//typ ---moze byt aj navratova hodnota
	char *data;						//data napr i = 50 v datach bude char "50";
	bool fce;						//true - je funkcia, false - je premenna	
	TList *args;					//argumenty je potreba robit jednosmerne viazany zoznam
	bool init;
	bool decl;
	locTable *ptr_loctable; 	//tabulka pre lokalne symboly
	char *class_name;				//nazov triedy ,ktorej patri symbol
	bool isstatic;					//true - globalna, false - lokalna
	struct Sym_item *nextptr;
}iSymbol;


typedef struct Hash_table{

	iSymbol *ptr;
}Hash_item;	

typedef Hash_item tHTable[Hash_table_size];

//----------------------------------------------------------
typedef struct Class_table {
	char *classname;
	tHTable *ptr;
	struct Class_table *next;
}Hash_class;

typedef Hash_class clHTable[Hash_table_size];


//extern tHTable *Main_table;
//extern clHTable *Class_table;

//local table
void sym_function_add_locals(iSymbol* funcsym,locTable* ptrloctable);
locTable* loc_table_init();
locSymbol* loc_symbol_init(char *data,int stype, bool isinit, char *classname);
void loc_table_insert(locTable* ptrloctable,locSymbol* new_locsymbol, char *data);
locSymbol* loc_symbol_search(locTable* ptrloctable,char *data);
//table containing classes
clHTable* class_init();
Hash_class* class_search(clHTable *clptr,char *classname);
void class_insert(clHTable *clptr, Hash_class *ptrclass);
Hash_class* make_class(tHTable* tab, char *classname);
//linked list
TList* linked_list_init();
void list_insert_first(TList *list, iSymbol *arg);
void list_insert_next(TList *list, iSymbol *arg);
//symbol
symbolType sym_type(Ttoken token);
void sym_copy_variable(iSymbol* ptrsym1, iSymbol* ptrsym2);
void function_add_args(iSymbol* funcsym, iSymbol* arg, int counter);
iSymbol* sym_variable_init(char *data, int stype, bool isinit, char *classname, bool isstat, bool isdecl);
iSymbol* sym_function_init(char *data, int stype, char *classname);
//table for class e.g. Main
void Htab_insert(tHTable* tab, iSymbol* newsymbol,char *data);
tHTable* HTab_init();
unsigned hash_function(char *data);
iSymbol* Htab_search(tHTable *ST, char *id);


/**********************************************************/

char* heapsort(char *ptr,unsigned int n);
int boyer_moore(const char *str, const char *pattern);

#endif
