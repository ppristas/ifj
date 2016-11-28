/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          ial.c                                                   *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *
   ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "ial.h"
#include "buildin.h"
#include "cleaner.h"
#include "error.h"
//#include "ilist.h"

//tHTable* Main_table;
//clHTable* Class_table;

/*
void buildin_args(iSymbol *buildinsym, char *data, int typ_s) {
	//TODO
}*/

int contain_me(iSymbol *funcsym, char *name) {
	TList *templist = funcsym->data->args;
	int currlist_len = 0;
	int it_contains = 0;

	if(templist == NULL)
		return 21;

	while((templist->first != NULL) && (currlist_len<=funcsym->data->arg_count)) {
		if((strcmp(templist->first->name, name) == 0)) {
			it_contains = 1;
			break;
		}
		else
			templist->first = templist->first->next;
			currlist_len++;
	}
	if(it_contains == 1)
		return 42;
	else
		return 21;
}
/**
 *      searches for symbol
 *
 * @param      ptrloctable  pointer to local table
 * @param      data         name of symbol
 *
 * @return     if found then returns pointer to symbol
 */
locSymbol* loc_symbol_search(locTable* ptrloctable,char *data) {
	unsigned index = hash_function(data);
	locSymbol* findlocsym = (*ptrloctable)[index].ptr;

	while(findlocsym != NULL) {
		if(strcmp(findlocsym->name,data) == 0) {
			return findlocsym;
		}
		findlocsym = findlocsym->nextptr;
	}
	return NULL;
}

/**
 *       inserts symbol into local table 
 *
 * @param      ptrloctable    pointer to local table
 * @param      new_locsymbol  symbol to be inserted
 */
void loc_table_insert(locTable* ptrloctable,locSymbol* new_locsymbol) {
	if(loc_symbol_search(ptrloctable,new_locsymbol->name) != NULL) {
		//do nothing
		error = INTERNAL_ERR;
		clearAll();
		return;
	}
	else {
		unsigned index = hash_function(new_locsymbol->name);
		locSymbol* pom = (*ptrloctable)[index].ptr;
		if(pom != NULL) {
			new_locsymbol->nextptr = (*ptrloctable)[index].ptr;
			(*ptrloctable)[index].ptr = new_locsymbol;
		}
		else {
			(*ptrloctable)[index].ptr = new_locsymbol;
		}
	}
}

/**
 *       linked list of arguements
 *
 * @param      locfuncsym  pointer to function
 * @param      name        name of arguement
 * @param      typ_s       type of arguement
 * @param      counter     counter of arguement
 */
void local_function_add_args(locSymbol* locfuncsym, char *name, int typ_s, int counter) {

    if(locfuncsym->data->arg_count == 0) {

        locfuncsym->data->args = mymalloc(sizeof(struct _TList));
        if(locfuncsym->data->args == NULL) {
            error = INTERNAL_ERR;
            clearAll();
            return;
        }

        //adds only first arguement
        locfuncsym->data->args = linked_list_init();
        list_insert_first(locfuncsym->data->args, name, typ_s);
        locfuncsym->data->arg_count = counter + 1;

        
    }
    else {

        //lets begin
        list_insert_next(locfuncsym->data->args, name, typ_s);
        locfuncsym->data->arg_count = counter + 1;
    }
}

/**
 *       initialization of local function symbol
 *
 * @param      data        name of symbol
 * @param      ptrsymdata  pointer to strucuture for additional data
 * @param      stype       type of symbol
 * @param      classname   name of class
 *
 * @return     pointer to symbol
 */
locSymbol* loc_symbol_function_init(char *data,  int stype, char *classname) {
	locSymbol* locsym = NULL;
	symData *ptrsymdata = NULL;
	locsym = mymalloc(sizeof(struct Loc_item));
	if(locsym == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}

	int len_name = strlen(data);
	locsym->name = mymalloc(len_name*sizeof(char) + 2);
	if(locsym->name == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	strcpy(locsym->name,data);
	locsym->name[strlen(data)+1] = '\0';

	int len_classname = strlen(classname);
	locsym->class_name = mymalloc(len_classname*sizeof(char) + 2);
	if(locsym->class_name == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	strcpy(locsym->class_name,classname);
	locsym->class_name[strlen(classname)+1] = '\0';

	ptrsymdata = mymalloc(sizeof(symData));
	if(ptrsymdata == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	locsym->data = ptrsymdata;
	locsym->data->type = stype;
	locsym->data->arg_count = 0;
	locsym->fce = true;
	locsym->data->args = NULL;
	locsym->data->instrPtr = NULL;
	locsym->init = false;
	locsym->decl = true;
	locsym->nextptr = NULL;
	return locsym;

}

/**
 *       initializatioon of local symbol variable
 *
 * @param      data        name of symbol
 * @param      ptrsymdata  pointer to structure for additional data
 * @param      stype       type of symbol
 * @param      isinit      bool init
 * @param      classname   name of class
 *
 * @return     pointer to symbol
 */
locSymbol* loc_symbol_init(char *data, int stype, bool isinit, bool isdecl, char *classname) {
	locSymbol* locsym = NULL;
	symData* ptrsymdata = NULL;
	locsym = mymalloc(sizeof(struct Loc_item));
	if(locsym == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}

	int len_name = strlen(data);
	locsym->name = mymalloc(len_name*sizeof(char) + 2);
	if(locsym->name == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	strcpy(locsym->name,data);
	locsym->name[strlen(data)+1] = '\0';

	int len_classname = strlen(classname);
	locsym->class_name = mymalloc(len_classname*sizeof(char) + 2);
	if(locsym->class_name == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	strcpy(locsym->class_name,classname);
	locsym->class_name[strlen(classname)+1] = '\0';

	ptrsymdata = mymalloc(sizeof(symData));
	if(ptrsymdata == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	
	locsym->data = ptrsymdata;
	locsym->data->type = stype;
	locsym->data->arg_count = 0;
	locsym->fce = NULL;
	locsym->data->args = NULL;
	locsym->data->instrPtr = NULL;
	locsym->init = isinit;
	locsym->decl = isdecl;
	locsym->nextptr = NULL;

	return locsym;
}

/**
 *       initialization of local table
 *
 * @return     pointer to the local table
 */
locTable* loc_table_init() {
	locTable* result = mymalloc(sizeof(Hash_local_item) * Hash_table_size);

	if(result == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	for(unsigned i = 0; i < Hash_table_size; i++) {
		(*result)[i].ptr = NULL;
	}
	return result;
}

/**
 *       searches for class
 *
 * @param      clptr      table of classes
 * @param      classname  name of class
 *
 * @return     pointer to the class with name
 */
Hash_class* class_search(clHTable *clptr,char *classname) {
	unsigned index = hash_function(classname);
	Hash_class *findclass = (*clptr)[index].next;

	while(findclass != NULL) {
		if(strcmp(findclass->classname,classname)== 0) {
			return findclass;
		}
		findclass = findclass->next;
	}
	return NULL;
}

/**
 *      links name of the class with the pointer for the class
 *
 * @param      tab        pointer for the  class
 * @param      classname  name of class
 *
 * @return     pointer for the class with name
 */
Hash_class* make_class(tHTable* tab, char *classname) {
	Hash_class* result = mymalloc(sizeof(Hash_class));
	if(result == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	int classname_len = strlen(classname);
	result->classname = mymalloc(classname_len*sizeof(char) + 2);
	if(result->classname == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
	strcpy(result->classname,classname);
	result->classname[strlen(classname)+1] = '\0';
	result->ptr = tab;
	result->next = NULL;

	return result;

}

/**
 *      inserts class into table
 *
 * @param      clptr     pointer to to table for classes
 * @param      ptrclass  pointer to the class
 */
void class_insert(clHTable *clptr, Hash_class *ptrclass) {

		unsigned index = hash_function(ptrclass->classname);
		Hash_class *pom = (*clptr)[index].next;
		if(pom != NULL) {
			ptrclass->next = (*clptr)[index].next;
			(*clptr)[index].next = ptrclass;
		}
		else {
			(*clptr)[index].next = ptrclass;
		}

}

/**
 *      initialization of table for classes
 *
 * @return     pointer to the table of classes
 */
clHTable* class_init() {
	clHTable* result = mymalloc(sizeof(Hash_class) * Hash_table_size);

	if(result == NULL) {
		return NULL;
	}
	for(unsigned i = 0; i < Hash_table_size; i++) {
		(*result)[i].next = NULL;
	}
	return result;
}

/**
 * deletes arguements
 *
 * @param      list  pointer to list
 */
void list_delete(TList *list) {
	TNode *deletenode = NULL;

	while(list->first != NULL) {
		list->act = list->first;
		list->first = list->first->next;
		deletenode = list->act;
		free(deletenode);
	}
	list->act = NULL;
}

/**
 * inserts first arguement of symbol function into list
 *
 * @param      list  pointer to list
 * @param      arg   pointer to arguement
 */
void list_insert_first(TList *list, char *name, int typ_s) {
	TNode *tmp = mymalloc(sizeof(TNode));
	if((tmp == NULL) || (list == NULL)) {
		fprintf(stderr, "NULL pointer\n");
		error = INTERNAL_ERR;
		clearAll();
		return;
	}

	int arg_len = strlen(name);
	tmp->name = mymalloc(arg_len*sizeof(char) + 2);
	if(tmp->name == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return;
	}
	strcpy(tmp->name,name);
	tmp->name[strlen(name)+1] = '\0';

	tmp->type = typ_s;
	tmp->next = list->first;
	list->first = tmp;
	list->act = list->first;
}

/**
 * inserts arguements of symbol function into list
 *
 * @param      list  pointer to list
 * @param      arg   pointer to arguement
 */
void list_insert_next(TList *list, char *name, int typ_s) {
	//new argument
	TNode *addnode = NULL;
	if(list->act != NULL) {
		if((addnode = mymalloc(sizeof(TNode))) == NULL) {
			error = INTERNAL_ERR;
			clearAll();
			return;
		}
		else {
			int arg_len = strlen(name);
			addnode->name = mymalloc(arg_len*sizeof(char) +2);
			if(addnode->name == NULL) {
				error = INTERNAL_ERR;
				clearAll();
				return;
			}
			strcpy(addnode->name,name);
			addnode->name[strlen(name)+1] = '\0';
			addnode->type = typ_s;
			
			addnode->next = NULL;
			list->act->next = addnode;
			list->act = addnode;
		}
	}
}

/**
 * initialization of linked list of arguements
 *
 * @return     pointer to list
 */
TList* linked_list_init() {
      TList *list = mymalloc(sizeof(TList));
      if(list == NULL) {
         error = INTERNAL_ERR;
         clearAll();
         return NULL;
      }
      list->first = NULL;
      list->act = NULL;

      return list;
}

/**
 * creates type of symbol from given token
 *
 * @param  token  structure of information from given token
 *
 * @return     type of symbol
 */
symbolType sym_type(Ttoken token) {
    if(strcmp(token.data,"int")==0) {
        return tInt;
    }
    else if(strcmp(token.data,"String")==0) {
        return tString;
    }
    else if(strcmp(token.data,"double")==0) {
        return tDouble;
    }
    else if(strcmp(token.data,"true")==0 || strcmp(token.data,"false")==0) {
    	return tBool;
    }
    else if(strcmp(token.data,"void")==0) {
    	return tNan;
    }
    else {
        fprintf(stderr, "Undefined type of token %s\n", token.data);
        error = INTERNAL_ERR;
        return tNan;
    }

}

/**
 * initializes symbol variable from given data
 *
 * @param      name		  name of symbol
 * @param	   stype      type of symbol
 * @param      isinit     bool value true-initialized false-not initialized
 * @param      classname  name of class which belongs to
 * @param      isstat     global/local variable
 *
 * @return     pointer to symbol
 */
iSymbol* sym_variable_init(char *data, int stype, bool isinit, char *classname, bool isstat, bool isdecl) {
    iSymbol* ptrsym = NULL;
    symData* ptrsymdata = NULL;
    ptrsym = mymalloc(sizeof(struct Sym_item));
    if(ptrsym == NULL) {
        error = INTERNAL_ERR;
        clearAll();
        return NULL;
    }
    int len_name = strlen(data);
    ptrsym->name = mymalloc(len_name*sizeof(char) + 2);
    if(ptrsym->name == NULL) {
        error = INTERNAL_ERR;
        clearAll();
        return NULL;
    }
    strcpy(ptrsym->name,data);
    ptrsym->name[strlen(data)+1] = '\0';

    int len_classname = strlen(classname);
	ptrsym->class_name = mymalloc(len_classname*sizeof(char) + 2);
    if(ptrsym->class_name == NULL) {
    	error = INTERNAL_ERR;
    	clearAll();
    	return NULL;
    }
    strcpy(ptrsym->class_name, classname);
    ptrsym->class_name[strlen(classname)+1] = '\0';

    ptrsymdata = mymalloc(sizeof(symData));
	if(ptrsymdata == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
    ptrsym->data = ptrsymdata;
    ptrsym->data->type = stype;
    ptrsym->data->arg_count = 0;
    ptrsym->fce = false;
    ptrsym->decl = isdecl;
    ptrsym->data->args = NULL;
    ptrsym->data->instrPtr = NULL;
    ptrsym->ptr_loctable = NULL;
    ptrsym->init = isinit;
    ptrsym->isstatic = isstat;
    ptrsym->nextptr = NULL;//(*tab)[index].ptr;

    return ptrsym;
}

void sym_function_add_locals(iSymbol* funcsym,locTable* ptrloctable) {
	funcsym->ptr_loctable = ptrloctable;
}

/**
 * initializes symbol function from given data
 *
 * @param 	   name 	  name of function
 * @param      data       count of arguements
 * @param[in]  stype      return type of function
 * @param      classname  name of class which belongs to
 *
 * @return     pointer to symbol
 */
iSymbol* sym_function_init(char *data, int stype, char *classname) {

    iSymbol* ptrsym = NULL;
    symData* ptrsymdata = NULL;
    ptrsym = mymalloc(sizeof(struct Sym_item));
    if(ptrsym == NULL) {
        error = INTERNAL_ERR;
        clearAll();
        return NULL;
    }
    int length = strlen(data);
    ptrsym->name = mymalloc(length*sizeof(char) + 2);
    if(ptrsym->name == NULL) {
        error = INTERNAL_ERR;
        clearAll();
        return NULL;
    }
    strcpy(ptrsym->name,data);
    ptrsym->name[strlen(data)+1] = '\0';

    int len_classname = strlen(classname);
    ptrsym->class_name = mymalloc(len_classname*sizeof(char) + 2);
    if(ptrsym->class_name == NULL) {
    	error = INTERNAL_ERR;
    	clearAll();
    	return NULL;
    }
    strcpy(ptrsym->class_name, classname);
    ptrsym->class_name[strlen(classname)+1] = '\0';

    ptrsymdata = mymalloc(sizeof(symData));
	if(ptrsymdata == NULL) {
		error = INTERNAL_ERR;
		clearAll();
		return NULL;
	}
    ptrsym->data = ptrsymdata;
    ptrsym->data->type = stype;
    ptrsym->data->arg_count = 0;
    ptrsym->ptr_loctable = NULL;
    ptrsym->fce = true;
    ptrsym->data->args = NULL;
    ptrsym->data->instrPtr = NULL;
    ptrsym->isstatic = false;
    ptrsym->init = false;
    ptrsym->nextptr = NULL;
    
    return ptrsym;
} 

/**
 * adds arguements into list for symbol of function
 *
 * @param      funcsym  pointer to symbol of function
 * @param      name     name of arguement
 * @param 	   typ_s	type of arguement
 * @param      counter  count of arguements
 */
void function_add_args(iSymbol* funcsym, char *name, int typ_s,int counter) {

    if(funcsym->data->arg_count == 0) {

        funcsym->data->args = mymalloc(sizeof(struct _TList));
        if(funcsym->data->args == NULL) {
            error = INTERNAL_ERR;
            clearAll();
            return;
        }

        //adds only first arguement
        funcsym->data->args = linked_list_init();
        list_insert_first(funcsym->data->args, name, typ_s);
        funcsym->data->arg_count = counter +1;        
    }
    else {

    	//funcsym->data->args = myrealloc(funcsym->data->args,sizeof(struct _TList) + sizeof(struct _TNode) * counter);
        //lets begin
        list_insert_next(funcsym->data->args, name, typ_s);
        funcsym->data->arg_count = counter +1;
    } 

}

/**
 * initializes table for class
 *
 * @return     pointer to table of class
 */
tHTable *HTab_init() {

	tHTable *result = mymalloc(sizeof(Hash_item) * Hash_table_size);

	if(result == NULL)
		//allocation error
		return NULL;
	for(unsigned i = 0; i < Hash_table_size; i++) {
		(*result)[i].ptr = NULL;
	}
	return result;
	
}

unsigned hash_function(char *data) {
	//unsigned int hash = 0;
	unsigned retval = 1;
	unsigned keylen = strlen(data);
	for(unsigned int i = 0; i < keylen; i++) {
		retval += data[i];
	}
	return(retval % Hash_table_size);
}


/**
 * inserts symbol into table
 *
 * @param      tab        pointer to table of symbols for class
 * @param      newsymbol  pointer to symbol which will be inserted
 */
void Htab_insert(tHTable* tab, iSymbol* newsymbol)
{
    if(Htab_search(tab,newsymbol->name) != NULL){
        //do nothing
        error = INTERNAL_ERR;
        clearAll();
        return;
    }
    else {
    	unsigned index = hash_function(newsymbol->name);    
        iSymbol *pom = (*tab)[index].ptr;
        if(pom != NULL){

			newsymbol->nextptr = (*tab)[index].ptr;
			(*tab)[index].ptr = newsymbol;
		}
		else {		
			(*tab)[index].ptr = newsymbol;
		}
	}

}

/**
 * searches for symbol by given name
 *
 * @param      ST    pointer to table of symbols of class
 * @param      id    name of symbol
 *
 * @return     pointer to symbol
 */
iSymbol *Htab_search(tHTable *ST,char *id) {
    unsigned index = hash_function(id);
    iSymbol *finditem = (*ST)[index].ptr;

    while(finditem != NULL) {
        if(strcmp(finditem->name, id) == 0) {
            //found
            return finditem;
        }
        finditem = finditem->nextptr;
        
    }
    //not found
    return NULL;
}

static void stifDown(char *str,   unsigned int left,   unsigned int right);

static void stifDown(char *str,   unsigned int left,   unsigned int right)
{
      unsigned int i=left;
      unsigned int j=2*i;
      unsigned int checker;

    char temp=str[i];
    checker=(j<=right);


    while (checker)
    {
        if (j<right)
        {
            if (str[j] < str[j+1])  //uprava <  10....1
            {
                j=j+1;
            }
        }

        if (temp >= str[j])   //uprava >= pre zoradenie 10....1
        {
             checker=0;
        }
        else
        {
            str[i]=str[j];
            i=j;
            j=2*i;
            checker=(j<=right);
        }
    }
    str[i]=temp;
}


char* heapsort(char *ptr,unsigned int n)
{
    unsigned int left,right;
    left=n/2;
    right=n;

    for(  unsigned int i = left; i>0; --i)
    {
        stifDown(ptr,i,right);
    }
    //unsigned int i=0;

    for(right = n;right > 1; --right)
    {
        char temp= ptr[1];
        ptr[1]=ptr[right];
        ptr[right]=temp;
        stifDown(ptr,1,right-1);
    }
    return (char*)&ptr[1];
}


//Hladanie najpravejsieho nematchnuteho prvku
static int find_jump(const char *pattern, const char c, int index){
    while(index >= 0){
        if (pattern[index] == c){
            return index;
        }
        index--;
    }
    return index;

}



//Hladanie podretazca boyer_moore algoritmom prvou heuristikou, funguje potrebujem vsak este odtestovat extremnejsie vstupy
int boyer_moore(const char *str, const char *pattern){
    int str_len=length(str);
    int pat_len=length(pattern);

    if(pat_len == 0){
        return 0;
    }

    if(pat_len > str_len){
        return -2;
    }

    int sIndex = pat_len-1;
    int pIndex = pat_len-1;

    while (sIndex < str_len){

        if(str[sIndex] != pattern[pIndex]){
            //osetrenie pripadu, ze nie je zhoda

            if(pIndex == pat_len-1){
                int j = find_jump(pattern, str[sIndex], pIndex-1);
                if (j == -1){
                    sIndex += pat_len;
                }
                else if (pat_len - 1 == 0){
                    sIndex += 1;
                }
                else{
                sIndex += pIndex - j ;
                }
            }

            //je zhoda, posun na zhodu
            else{
                sIndex += pat_len - (pat_len - pIndex);
            }
            pIndex = pat_len - 1;
        }

        else{
            if(pIndex == 0){
                return sIndex;
            }
            sIndex--;
            pIndex--;
        }


    }

    return -1;

}

