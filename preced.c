/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          predec.c                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/

   #include <ctype.h>
   #include <stdio.h>
   #include "error.h"
   #include "preced.h"
   #include "cleaner.h"
   #include "stack.h" 

   #define TAB_SIZE (16)

   static const char precedense_table[TAB_SIZE][TAB_SIZE] = 
   {/*     +    -    *    /    (    )   id   idf   <    >   <=   >=   ==   !=    ,    $  */
/* +  */ {'>', '>', '<', '<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'},   
/* -  */ {'>', '>', '<', '<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'},   
/* *  */ {'>', '>', '>', '>', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'},  
/* /  */ {'>', '>', '>', '>', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'},  
/* (  */ {'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '>', 'E'},   
/* )  */ {'>', '>', '>', '>', 'E', '>', 'E', 'E', '>', '>', '>', '>', '>', '>', '=', '>'}, 
/*id  */ {'>', '>', '>', '>', 'E', '>', 'E', 'E', '>', '>', '>', '>', '>', '>', '>', '>'}, 
/*idf */ {'E', 'E', 'E', 'E', '=', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'}, 
/* <  */ {'<', '<', '<', '<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'}, 
/* >  */ {'<', '<', '<', '<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'}, 
/* <= */ {'<', '<', '<', '<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'}, 
/* >= */ {'<', '<', '<', '<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'}, 
/* == */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '<', '<', '<', '>', '>', '>', '>'}, 
/* != */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '<', '<', '<', '>', '>', '>', '>'}, 
/* ,  */ {'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '=', 'E'}, 
/* $  */ {'<', '<', '<', '<', '<', 'E', '<', '<', '<', '<', '<', '<', '<', '<', 'E', 'E'},  
};


int catch_index(int *count){
	
	switch(token.stav){
		case S_PLUS:
			prem = PLUS;
			break;
		case S_MINUS:
			prem = MINUS;
			break;
		case S_MULTI:
			prem = KRAT;
			break;
		case S_DIV:
			prem = DELENE;
			break;
		case S_MEN:
			prem = MENSIE;
			break;
		case S_MENROV:
			prem = MENROV;
			break;
		case S_VAC:
			prem = VACSIE;
			break;
		case S_VACROV:
			prem = VACROV;
			break;
		case S_ROVNY:
			prem = EQUAL;
			break;
		case S_NEROV:
			prem = NEQUAL;
			break;
		case S_LZAT:
			prem = LZATV;
			(*count)++;
			break;
		case S_PZAT:
			prem = PZATV;
			(*count)--;
			break
		case S_ID:
			prem = ID;
	
		
			break;
		case S_DOUBLE:
		case S_INT:
		case S_EXP:
			prem = ID;

			break;
		case S_STRING:
			break;
		case S_CIARKA:
		case S_SEMICOLON:
			prem = DOLAR;
			break;
		default:
			error = SYNTAX_ERR;
			clearAll();
			break;
	}
}

int expresion_parser()
{
   int bracket_counter = 0;
   tStack Stack1;
   stackInit(&Stack1);  //zasobnik pre terminaly
   tStack Stack2;
   stackInit(&Stack2);

   bool readToken = true;

   stackPush(&Stack1, DOLAR); 
	
   
//	int a = stackTopPop(&Stack1);
	/*
	do{
		if(readToken){
			
		}
		switch (precedense_table[][]){	//case by takto fakci uz "len" doplnit :D
			case '=':
			//	stackPush(&Stack1, ) // premenna ktora bude aktualna
				break;
			case '<':
				printf("kk\n");
				break;
			case '>':
				printf("kkk\n");
				break;
			case 'E':
				error = SYNTAX_ERR;
				clearAll();
				break;
		}
	}while();*/
   (void)readToken;
   return 420;
}

