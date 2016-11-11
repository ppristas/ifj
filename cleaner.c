


#include "error.h"
#include <stdio.h>
#include "cleaner.h"
#include <stdlib.h>
#include "debug.h"
#include "scaner.h"

tMemoryPtr Mem;

/*
 *function: inicializacia
 */
void initCleaner(){				
	/*Mem->Act = NULL;
	Mem->Act->data = NULL;
	Mem->First = NULL;
Mem->First->data = NULL;*/
	if( (Mem = malloc(sizeof(struct tMemory))) == NULL){
		return;
	}
	Mem->First = NULL;
	Mem->Last = NULL;

}

/*
 *	function: allokuje pamat a prida ukazatel na nu do svojej datovej sturktury
 *  params: velkost ktoru chcete alokovat
 *
 */
void *mymalloc(unsigned int size){
	
	tAdrPtr pom;
	
	if( (pom = malloc(sizeof(struct tAdr))) == NULL){
		error = INTERNAL_ERR;
		return NULL;
	}
	void* tmp = malloc(size);
	if(tmp != NULL){
		pom->data = tmp;
		pom->size = size;
		pom->rPtr = NULL;
		pom->lPtr = NULL;
		if( Mem->First == NULL){	
			Mem->First = pom;
			Mem->Last = pom;
		}else{
			pom->lPtr = Mem->Last;
			Mem->Last->rPtr = pom;
			Mem->Last = pom;			
		}
	}else{
		error = INTERNAL_ERR;
		return NULL;
	}	
	return tmp;
			
}

/*
 *	funciton: realokuje pamat
 *	params: adresa ktoru treba realokovat, a velkost o kolko treba zvacsit miesto
 */
void *myrealloc(void *adress,unsigned int size){

	void *pom;
	if(adress == NULL){		//pridam 
		pom = mymalloc(size);
		if(pom == NULL){
			return NULL;
		}
		return pom;								
	}
	tAdrPtr tmpPtr = Mem->First;
	while(tmpPtr != NULL){
		if( tmpPtr->data == adress){
			tmpPtr->size +=size;
			tmpPtr->data = realloc(tmpPtr->data,tmpPtr->size);
			return tmpPtr->data;
		}
		tmpPtr = tmpPtr->rPtr;
	}
	return NULL;
	
}

/*
 *	function: uvolni alokovanu pamat
 *	params: uvolnovana pamat
 *//*
void myfree(void *uk){
	if(Mem->First != NULL){
		tAdrPtr pom = Mem->First;
		while(pom != NULL){
			if(pom->data == uk){
				break;	
			}
			pom = pom->rPtr;	
		}
		if(pom != NULL){
			if(Mem->First 
		}
	}
}*/


/*
 *	function: uvolni vsetku naalokovanu pamat
 *
 */
void clearAll(){
	tAdrPtr tmpPtr;

	printf("tmp cleaned\n");
	tmpPtr = Mem->First;
	while(tmpPtr != NULL){
		Mem->First = Mem->First->rPtr;
		if(tmpPtr->data != NULL)
			free(tmpPtr->data);
		free(tmpPtr);
		tmpPtr = Mem->First;
	}
	free(Mem);
	
	if((fclose(file)) == EOF)
		error = INTERNAL_ERR;
	free(filename);
	free(token.data);	
	exit(error);
}





