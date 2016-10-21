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

void HTab_init(HTab_table* ptrht) {

   if(ptrht != NULL) {
      for(int i = 0; i < HTSIZE; i++) {
         (*ptrht)[i] = NULL;
      }
   }
}

/*HTab_table* HTab_insert(HTab_table* ptrht,Ttoken token) {

}*/

HTab_listitem_sym* HTab_search(HTab_table* ptrht,Ttoken token) {

   HTab_listitem_sym* ptrsym = NULL;
   HTab_listitem_sym* tmp;
   int i = hash_function(token);
   if(ptrht != NULL) {
      tmp = (*ptrht)[i];
      while(tmp != NULL) {
         if(tmp->token.data == token.data) {
            ptrsym = tmp;
            return ptrsym;
         }
         else {
            tmp = tmp->ptrnext;
         }
      }
      return NULL;//not found
   }
   return NULL;
}

void HTab_insert(HTab_table* ptrht,Ttoken token) {
   HTab_listitem_sym* ptrsym = NULL;
   HTab_listitem_sym* newsym;

   int i;
   if(ptrht != NULL) {
      ptrsym = HTab_search(ptrht,token);
      if(ptrsym == NULL) {
         i = hash_function(token);
         if((newsym = (struct HTab_listitem_sym*)malloc(sizeof(struct HTab_listitem_sym))) == NULL) {
            /*allocation error*/
         }
         else {
            newsym->token.data = token.data;
            newsym->token.stav = token.stav;

            newsym->ptrnext = (*ptrht)[i];
            (*ptrht)[i] = newsym;
         }
      }
      else
         //element is in the table update data
         ptrsym->token.data = token.data;
   }
}

int hash_function(Ttoken token) {
   int retval = 1;
   int keylen = strlen(token.data);
   for(int i = 0 ; i < keylen; i++) {
      retval += token.data[i];
   }
   return(retval % HTSIZE);
}

void HTab_free(HTab_table* ptrht) {


   if(ptrht != NULL) {
      for(int i = 0; i < HTSIZE; i++) {
         HTab_listitem_sym *tmp = (*ptrht)[i];
         HTab_listitem_sym *help;
         while(tmp != NULL) {
            help = tmp;
            tmp = tmp->ptrnext;
            free(help);
         }
         (*ptrht)[i] = NULL;

      }
   }
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