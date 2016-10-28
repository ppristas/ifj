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
#include "scaner.h"
#include "ial.h"

HTab_t* HTab_init(unsigned size) {

	HTab_t* result = malloc(sizeof(HTab_t)+size*sizeof(HTab_listitem));
   
	if(result == NULL)
		/*allocation error*/
		return NULL;
	result->htable_size = size;
	for(unsigned i = 0; i < size; i++) {
		result->list[i] = NULL;
	}
	return result;
	free(result);

}

unsigned hash_function(unsigned size, Ttoken token) {
	//unsigned int hash = 0;
	unsigned retval = 1;
	unsigned keylen = strlen(token.data);
	for(unsigned int i = 0; i < keylen; i++) {
		retval += token.data[i];
	}
	return(retval % size);
}

HTab_listitem* HTab_insert(HTab_t* ptrht, Ttoken token) {
	unsigned ind = hash_function(ptrht->htable_size,token);
	HTab_listitem* item_ptr = NULL;
	HTab_listitem* item = ptrht->list[ind];
	HTab_listitem* nextitem;

	if(item == NULL) {
		nextitem = malloc(sizeof(HTab_listitem)+sizeof(char)*(strlen(token.data)+1));

		if(nextitem == NULL)
			/*allocation error*/
			return NULL;
		else {
			//printf("HERE\n");
			//printf("%s\n", token.data);
			//memcpy(nextitem->token.data,token.data,strlen(token.data)+1);
			int length = strlen(token.data);
            nextitem->token.data = malloc(length * sizeof((char) +2));
            strcpy(nextitem->token.data,token.data);
            nextitem->token.data[length] = '\0';
            nextitem->token.stav = token.stav;
			//printf("HERE AFTER\n");
			nextitem->ptrnext = NULL;

			item = ptrht->list[ind] = nextitem;
			
			nextitem = NULL;
			if(item == NULL)
				return NULL;
		}
	}
	else {
		while(item != NULL) {
			if(strcmp(item->token.data,token.data) == 0) {
				//if found
				item_ptr = item;
				break;
			}
			else {
				//next item
				item_ptr = item;
				item = item->ptrnext;
			}
		}
		if(item_ptr != NULL && item != item_ptr) {
			//not found insert next item
			nextitem = malloc(sizeof(HTab_listitem*)+sizeof(char)*(strlen(token.data)+1));
			if(nextitem == NULL)
				/*allocation error*/
				return NULL;
			else {
				//memcpy(nextitem->token.data,token.data,strlen(token.data)+1);
				int length = strlen(token.data);
	            nextitem->token.data = malloc(length * sizeof((char) +2));
	            strcpy(nextitem->token.data,token.data);
	            nextitem->token.data[length] = '\0';
	            nextitem->token.stav = token.stav;

				nextitem->ptrnext = NULL;
				item = nextitem;
				if(item == NULL)
					return NULL;
				item_ptr->ptrnext = item;
			}
		}
	}
	return item;
}

void HTab_free(HTab_t* ptrht) {
	if(ptrht != NULL) {
		HTab_clear(ptrht);
		free(ptrht);
	}
}

void HTab_clear(HTab_t* ptrht) {
	HTab_listitem* item = NULL;

	for(unsigned i = 0; i < ptrht->htable_size; i++) {
		while((item = ptrht->list[i]) != NULL) {
			HTab_remove(ptrht,token);
		}		
	}
	free(item);
}

void HTab_remove(HTab_t* ptrht,Ttoken token) {
	unsigned ind = hash_function(ptrht->htable_size,token);
	HTab_listitem* item_ptr = ptrht->list[ind];
	HTab_listitem* item = ptrht->list[ind];

	while(item != NULL) {
		//compare keys from table
		if(strcmp(item->token.data,token.data) == 0) {
			//printf("GOT IN REMOVE\n");
			if(item_ptr == item) {
				ptrht->list[ind] = item->ptrnext;
			}
			item_ptr->ptrnext = item->ptrnext;
			free(item);
			break;
		}
		else {
			item_ptr = item;
			item = item->ptrnext;
		}
	}
	item->ptrnext = NULL;
}



/**
 * Initialization of hash table
 *
 * @param  size  size of hash table
 *
 * @return     pointer to structure or NULL if error occurs
 */
/*HTab_t* HTab_init(HTab_t* ptrht) {
   HTab_t* result = malloc(sizeof(HTab_t)+HTSIZE*sizeof(HTab_listitem));
   if(result == NULL) {
      //not enough memory for allocation
      return NULL;
   }
   result->HTab_size = HTSIZE;
   for(unsigned i = 0; i < result->HTab_size; i++) {
      //initializing every item of table to NULL
      result->list[i] = NULL;
   }
   return result;
} */

/**
 * Hash function for table
 *
 * @param   data         string which will be used for calculation
 * @param   HTab_size   size of table                     
 *
 * @return              returns calculated hash
 */
/*unsigned int hash_function(HTab_t* ptrht,char *data) {
   unsigned retval = 1;
   int keylen = strlen(data);
   for(int i = 0; i < keylen; i++) {
      retval += data[i];
   }
   return(retval % ptrht->HTab_size);
} */

/*
void HTab_free(HTab_t* ptrht) {

   HTab_listitem* item = NULL;
   HTab_listitem* tmp;

   ptrht->HTab_size = HTSIZE;

   if(HT != NULL) {
      //looping through list[index]
      for(unsigned i = 0; i < ptrht->HTab_size; i++) {
         //looping through items
         while((item = ptrht->list[i]) != NULL) {
            tmp = item;
            item = item->ptrnext;
            free(tmp);
         }
         ptrht->list[i] = NULL;
      }
   }
}
*/

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
            if (str[j] > str[j+1])  //uprava <  10....1
            {
                j=j+1;
            }
        }

        if (temp < str[j])   //uprava >= pre zoradenie 10....1
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

void heapsort(char *ptr,char *str_ret,unsigned int n)
{
    unsigned int left,right;
    left=n/2;
    right=n;

    for(  unsigned int i = left; i>0; --i)
    {
        stifDown(ptr,i,right);
    }

    unsigned int i=0;

    for(right = n;right > 1; --right)
    {
        str_ret[i++]=ptr[1];
        ptr[1]=ptr[right];
        stifDown(ptr,1,right-1);
    }
    str_ret[i]=ptr[1];
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