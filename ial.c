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

tHTable* Main_table;

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

void list_insert_first(TList *list, iSymbol *arg) {
	TNode *tmp = mymalloc(sizeof(TNode));
	if((tmp == NULL) || (list == NULL)) {
		fprintf(stderr, "NULL pointer\n");
		error = INTERNAL_ERR;
		clearAll();
		return;
	}

	tmp->symarg = arg;
	tmp->next = list->first;
	list->first = tmp;
	//nastavenie aktivity
	list->act = list->first;
}

void list_insert_next(TList *list, iSymbol *arg) {
	//novy argument
	TNode *addnode = NULL;
	if(list->act != NULL) {
		if((addnode = mymalloc(sizeof(TNode))) == NULL) {
			error = INTERNAL_ERR;
			clearAll();
			return;
		}
		else {
			addnode->symarg = arg;
			addnode->next = list->act->next;
			list->act->next = addnode;
			list->act = list->act->next;
		}
	}
}

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
    else {
        //fprintf(stderr, "Undefined type of token %s\n", token.data);
        error = INTERNAL_ERR;
        return tNan;
    }

}

/**
 * copy data from ptrsym2 to ptrsym1
 * @param      ptrsym1  destination
 * @param      ptrsym2  source
 */
void sym_copy_variable(iSymbol* ptrsym1, iSymbol* ptrsym2) {
	if(ptrsym1->init == true || ptrsym1->type != ptrsym2->type) {
		error = SEMANTIC_PROG_ERR;
		clearAll();
		return;
	}

	int length_data1 = strlen(ptrsym1->data);
	int length_data2 = strlen(ptrsym2->data);

	if(length_data2 > length_data1) {
		ptrsym1->data = myrealloc(ptrsym1->data,sizeof(char)*length_data2);
	}
	ptrsym1->data = ptrsym2->data;
	ptrsym1->init = true;

	return;
}

iSymbol* sym_variable_init(char *data, int stype, bool isinit, char *classname) {
    iSymbol* ptrsym = NULL;
    ptrsym = mymalloc(sizeof(struct Sym_item));
    if(ptrsym == NULL) {
        error = INTERNAL_ERR;
        clearAll();
        return NULL;
    }
    ptrsym->name = mymalloc(strlen(data)*sizeof(char) + 2);
    if(ptrsym->name == NULL) {
        error = INTERNAL_ERR;
        clearAll();
        return NULL;
    }
    strcpy(ptrsym->name,data);
    ptrsym->name[strlen(data)+1] = '\0';

	ptrsym->class_name = mymalloc(strlen(classname)*sizeof(char) + 2);
    if(ptrsym->class_name == NULL) {
    	error = INTERNAL_ERR;
    	clearAll();
    	return NULL;
    }
    strcpy(ptrsym->class_name, classname);
    ptrsym->class_name[strlen(classname)+1] = '\0';

    ptrsym->type = stype;
    ptrsym->data = NULL;
    ptrsym->fce = false;
    ptrsym->args = NULL;
    ptrsym->init = isinit;
    ptrsym->nextptr = NULL;//(*tab)[index].ptr;

    return ptrsym;
}

iSymbol* sym_function_init(char *data, int stype, char *classname) {

    iSymbol* ptrsym = NULL;
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

    ptrsym->class_name = mymalloc(strlen(classname)*sizeof(char) + 2);
    if(ptrsym->class_name == NULL) {
    	error = INTERNAL_ERR;
    	clearAll();
    	return NULL;
    }
    strcpy(ptrsym->class_name, classname);
    ptrsym->class_name[strlen(classname)+1] = '\0';

    ptrsym->type = stype;
    ptrsym->data = NULL;
    ptrsym->fce = true;
    ptrsym->args = NULL;
    ptrsym->init = false;
    ptrsym->nextptr = NULL;
    
    return ptrsym;
} 

void function_add_args(iSymbol* funcsym, iSymbol* arg, int counter) {

    char str[15];
    sprintf(str,"%d",counter);

    if(funcsym->data == NULL) {
        int length = strlen(str);
        funcsym->data = mymalloc(length*sizeof(char) + 2);
        if(funcsym->data == NULL) {
            error = INTERNAL_ERR;
            clearAll();
            return;
        }
        strcpy(funcsym->data,str);
        funcsym->data[strlen(str)+1] = '\0';

        funcsym->args = mymalloc(sizeof(struct Sym_item));
        if(funcsym->args == NULL) {
            error = INTERNAL_ERR;
            clearAll();
            return;
        }

        //adds only first arguement
        funcsym->args = linked_list_init();
        list_insert_first(funcsym->args, arg);
        printf("%s\n", funcsym->args->first->symarg->name);

        
    }
    else {

        int length_new = strlen(str);
        int length_prev = strlen(funcsym->data);

        if(length_new > length_prev) {
            funcsym->data = myrealloc(funcsym->data,sizeof(char)*length_new);
            if(funcsym->data == NULL) {
                error = INTERNAL_ERR;
                clearAll();
                return;
            }
            strcpy(funcsym->data,str);
            funcsym->data[strlen(str)+1] = '\0';
        }
        strcpy(funcsym->data,str);
        funcsym->data[strlen(str)+1] = '\0';

        //lets begin
        list_insert_next(funcsym->args, arg);
        printf("%s\n", funcsym->args->act->symarg->name);
    } 

}

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


//vlozi symbol do tabulky ak sa nachadza tak prepise datovu cast
void Htab_insert(tHTable* tab, iSymbol* newsymbol , char *data)
{
    if(Htab_search(tab,newsymbol->name) != NULL){
        //prepisem data
        
        if(newsymbol->data == NULL){
            int length = strlen(data);
            newsymbol->data = mymalloc(length*sizeof(char) + 2);
            strcpy(newsymbol->data,data);
            newsymbol->data[strlen(data)+1] = '\0';
        }else{
            int length_new = strlen(data);
            int length_prev = strlen(newsymbol->data);

            if(length_new > length_prev) {
                newsymbol->data = myrealloc(newsymbol->data,sizeof(char)*length_new);    
                strcpy(newsymbol->data,data);
                newsymbol->data[strlen(data)+1] = '\0';
            }

        }

    }else{
        unsigned index = hash_function(newsymbol->name);    
        iSymbol *pom = (*tab)[index].ptr;
        if(pom != NULL){

			newsymbol->nextptr = (*tab)[index].ptr;
			(*tab)[index].ptr = newsymbol;
//			printf("\n\n----%s----- \n\n",pom->nextptr->name);	
		}
		else{		
//	printf("%ld -%s-\n",strlen(tt->name),tt->name);
		(*tab)[index].ptr = newsymbol;
		}
	}


}

//vrati ukazatel na symbol v pripade najdenia inak vracia NULL
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

