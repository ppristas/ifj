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
	void *args;			//argumenty je potreba robit jednosmerne viazany zoznam
	bool init;
	struct Sym_item *nextptr;
}iSymbol;


typedef struct Hash_table{

	iSymbol *ptr;
}Hash_item;	

typedef Hash_item tHTable[Hash_table_size];


extern tHTable *Main_table;

//symbol
symbolType sym_type(Ttoken token);
iSymbol* sym_variable_init(char *data, int stype, bool isinit);
iSymbol* sym_function_init(char *data, int stype);
//table
void Htab_insert(tHTable* tab, iSymbol* newsymbol,char *data);
tHTable* HTab_init();
unsigned hash_function(char *data);
iSymbol* Htab_search(tHTable *ST, char *id);

/**********************************************************/

//void HTab_free(HTab_t* ptrht);
//void HTab_clear(HTab_t* ptrht);
//void HTab_remove(HTab_t* ptrht,Ttoken token);

char* heapsort(char *ptr,unsigned int n);
int boyer_moore(const char *str, const char *pattern);

#endif
