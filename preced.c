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


int catch_index(SAData *pom,int *count){
	
	switch(token.stav){
		case S_PLUS:
			pom->indexibus = PLUS;
			break;
		case S_MINUS:
			pom->indexibus = MINUS;
			break;
		case S_MULTI:
			pom->indexibus = KRAT;
			break;
		case S_DIV:
			pom->indexibus = DELENE;
			break;
		case S_MEN:
			pom->indexibus = MENSIE;
			break;
		case S_MENROV:
			pom->indexibus = MENROV;
			break;
		case S_VAC:
			pom->indexibus = VACSIE;
			break;
		case S_VACROV:
			pom->indexibus = VACROV;
			break;
		case S_ROVNY:
			pom->indexibus = EQUAL;
			break;
		case S_NEROV:
			pom->indexibus = NEQUAL;
			break;
		case S_LZAT:
			pom->indexibus = LZATV;
			(*count)++;
			break;
		case S_PZAT:
			pom->indexibus = PZATV;
			(*count)--;
			break;
		case S_ID:
			pom->indexibus = ID;
	
		
			break;
		case S_DOUBLE:
		case S_INT:
		case S_EXP:
			pom->indexibus = ID;

			break;
		case S_STRING:
			break;
		case S_CIARKA:
		case S_SEMICOLON:
			pom->indexibus = DOLAR;
			printf("ke\n");
			break;
		default:
			error = SYNTAX_ERR;
			clearAll();
			break;
	}
	printf("%d\n",pom->indexibus);
	return pom->indexibus;
}


void show_stacks(tStack *s1,tStack *s2)
{
    SAData pom1, pom2;
    printf("\t1.zasobnik\t2.zasobnik\n");
	int i = 0;
	int a = 0;

	stackTop(s1,&pom1);
	stackTop(s2,&pom2);
    while((!(stackEmpty(s1)) || (!(stackEmpty(s2))))){
     		
	    stackTopPop(s1,&pom1);
        stackTopPop(s2,&pom2);
		if(!stackEmpty(s1)){
			printf("\t[ %d ]",pom1.indexibus);
		}else{
			i++;
			if(i == 1)
	            printf("\t[ %d ]",pom1.indexibus);
			else
			printf("\t");
		}
		if(!stackEmpty(s2)){
			printf("\t\t[ %d ]\n",pom2.indexibus);
		}else{
			a++;
			if(a == 1)
	            printf("\t\t[ %d ]",pom2.indexibus);
			printf("\n");
		}   	
	 }
}



int expresion_parser()
{
   int bracket_counter = 0;
	
	//oba zasobniky
	tStack Stack1;
	stackInit(&Stack1);  //zasobnik pre terminaly
	tStack Stack2;
	stackInit(&Stack2);

	//ci mam citat dalsi token alebo nie
	bool readToken = true;

	//pravy index;
	SAData right_index, left_index;
	SAData pom;

	pom.sym_data.name = NULL;
	pom.indexibus = DOLAR;

	stackPush(&Stack1,&pom);
	get_token();
	
	catch_index(&right_index,&bracket_counter);
	
//	do{
		if(readToken){
			catch_index(&right_index,&bracket_counter);
			
		}
		stackTop(&Stack1,&left_index);
			

		switch (precedense_table[left_index.indexibus][right_index.indexibus]){	//case by takto fakci uz "len" doplnit :D
			case '=':
				readToken = true;
				stackPush(&Stack1,&right_index) 
				break;
			case '<':
				readToken = true;
												//zatial bez druheho stacku
				pom.indexibus = L_HANDLE;
				pushStack(&Stack1,&pom);
				pushStack(&Stack1,&right_index);
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

		



//	}while();*/
//   (void)readToken;
   return 420;
}


