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
   //#include "ial.h"
   #include "ilist.h"
   #include "scaner.h"
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
/* <  */ {'<', '<', '<', '<', '<', '>', '<', '<', 'E', 'E', 'E', 'E', 'E', 'E', '>', '>'},
/* >  */ {'<', '<', '<', '<', '<', '>', '<', '<', 'E', 'E', 'E', 'E', 'E', 'E', '>', '>'},
/* <= */ {'<', '<', '<', '<', '<', '>', '<', '<', 'E', 'E', 'E', 'E', 'E', 'E', '>', '>'},
/* >= */ {'<', '<', '<', '<', '<', '>', '<', '<', 'E', 'E', 'E', 'E', 'E', 'E', '>', '>'},
/* == */ {'<', '<', '<', '<', '<', '>', '<', '<', 'E', 'E', 'E', 'E', 'E', 'E', '>', '>'},
/* != */ {'<', '<', '<', '<', '<', '>', '<', '<', 'E', 'E', 'E', 'E', 'E', 'E', '>', '>'},
/* ,  */ {'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '=', 'E'},
/* $  */ {'<', '<', '<', '<', '<', 'E', '<', '<', '<', '<', '<', '<', '<', '<', 'E', 'E'},
};


/*
 * function: ziskanie indexu do preced.table zo stavu v aktualnom tokene
 * params: pom -> premenna do ktorej sa zapise index
 * params: count -> pocet zatvoriek (na detekovanie konca volania funkcie
 */
int catch_index(SAData *pom,int *count){

	switch(token2.stav){
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
			//TODO TOKEN2.data
			//overovat z tabulky symbolov ci sa tam nachadza
      //ziskat z find_symbol ukazatel s token2.data
      //
			//

			break;
		case S_DOUBLE:
		case S_INT:
		case S_EXP:
      //key = vygenerovat sprintfom
      //data->arg_count = 0;
      //data->args = NULL;
      //data->symbolType = tDouble;
      //data->ptr_union.d = atoi(token2.data);

			pom->indexibus = ID;

      //pom->sym_data->data = /priradim si vytvorenu premennu
			//TODO
			//pretypovanie na double
			//vlozit do tabulky symbolov
			//preconvertovat cez atoi
			break;
		case S_STRING:
      //name vygenerovat
      //key = vygenerovat sprintfom
      //data->arg_count = 0;
      //data->args = NULL;
      //data->symbolType = tString;
      //data->ptr_union.str = token.data;

			pom->indexibus = ID;
      //pom->sym_data->data = /priradim si vytvorenu premennu
			//TODO
			//vlozit do tabulky symbolov

			break;
		case S_CIARKA:
		case S_SEMICOLON:
			pom->indexibus = DOLAR;
			break;
		default:
			error = SYNTAX_ERR;
			clearAll();
      return error;
			break;
	}
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

void show_stacks2(tStack *s1,tStack *s2){
	SAData t;
	printf("stack1\n");
	if(stackEmpty(s1)){
		printf("je prazdny\n\n");
	}
	while(!stackEmpty(s1)){
		stackTopPop(s1,&t);
		printf("\t[ %d ]\n",t.indexibus);
	}
	printf("\n\nstack2\n");
	if(stackEmpty(s2))
		printf("je prazdny\n\n");
	while(!stackEmpty(s2)){
		stackTopPop(s2,&t);
		printf("\t[ %d ]\n",t.indexibus);
	}
}



int reduction(tStack *stack1,tStack *stack2){
	SAData hhelp1;
	SAData hhelp2;
	SAData hhelp3;
	SAData hhelp4;
	SAData neterminal;

  //eInstrType instruction;
	//odkladame na druhy zasobnik;
	stackTop(stack1,&hhelp1);
	while((hhelp1.indexibus != L_HANDLE) && (!stackEmpty(stack1))){
		stackTopPop(stack1,&hhelp2);
		stackPush(stack2,&hhelp2);
		stackTop(stack1,&hhelp1);
	}

	if( hhelp1.indexibus == L_HANDLE){
		stackTopPop(stack2,&hhelp2);

		//******* E -> id
		//*******
		//*******
		if(hhelp2.indexibus == ID){

			stackPop(stack1);		//dam dofaka leftHandle


			neterminal = hhelp2 ;
			neterminal.indexibus = NETERM;
            //TODO vlozit do tabulky sybolov



			stackPush(stack1,&neterminal);
			return SUCCESS;

		//******
		//****** E->E op E
		//******
		}else if(hhelp2.indexibus == NETERM){
			//na top stack1 je L_HANDLE!!TODO
			if(stackEmpty(stack2)){
				error = SYNTAX_ERR;
				clearAll();
				return error;
			}
			stackTopPop(stack2,&hhelp3);
			//musi byt operator za neterminalom
			if( hhelp3.indexibus == PLUS    ||
			    hhelp3.indexibus == MINUS   ||
			    hhelp3.indexibus == KRAT    ||
				  hhelp3.indexibus == DELENE  ||
				  hhelp3.indexibus == MENSIE  ||
				  hhelp3.indexibus == VACSIE  ||
				  hhelp3.indexibus == MENROV  ||
				  hhelp3.indexibus == VACROV  ||
				  hhelp3.indexibus == EQUAL   ||
				  hhelp3.indexibus == NEQUAL   ){
					;
			}else{
				error = SYNTAX_ERR;
				errorFce();
				clearAll();
				return error;
			}

			 //generovanie instrukcie TODO cez jednoduchy switch (hhelp3.indexibus){ }
			/*
			switch(hhelp3.indexibus){
				case PLUS:
          instruciton = I_ADD;
          break;
				case MINUS:
          instruction = I_SUB;
          break;
				case KRAT:
          instruction = I_MUL;
          break;
  			case DELENE:
          instruction = I_DIV;
          break;
				case MENSIE:
          instruction = I_LESS;
          break;
      	case VACSIE:
          instruction = I_BIGGER;
          break;
				case MENROV:
          instruction = I_LESSEQ;
          break;
				case VACROV:
          instruction = I_BIGGEREQ;
          break;
				case EQUAL:
          instruction = I_EQ;
          break;
				case NEQUAL:
          instruction = I_NOTEQ;
          break;
        default:
          error = SEMANTIC_ERR;
          clearAll();
          return error;
          break;
			}

			*/

			if(stackEmpty(stack2)){		//ak je prazdny nastava chyba -> nedocitali sme pravidlo
                error = SYNTAX_ERR;
                clearAll();
                return error;
            }
			//mal by byt dalsi neterminal

			stackTopPop(stack2,&hhelp4);

			//mozem redukovat, kedze je prazdny stack2 a bol neterminal na vrchole
			if((hhelp4.indexibus == NETERM) && (stackEmpty(stack2))){
				//redukujem
				//TODO vlozit do tabulky symbolov
				// a do instrukcnej pasky
				//TODO zistovat ci je string a double?

				//TODO ak je jeden z NETERMINALOV string a operator je + tak dam konkatenaciu

				error = SUCCESS;
				stackPop(stack1);
				neterminal.indexibus = NETERM;
				stackPush(stack1,&neterminal);
				return error;
			}else{
				error = SYNTAX_ERR;
        clearAll();
        return error;
      }

		//*****
		//***** E->(E)
		//*****
		}else if(hhelp2.indexibus == LZATV){
			if(stackEmpty(stack2)){
				error = SYNTAX_ERR;
        clearAll();
        return error;
			}
			stackTopPop(stack2,&hhelp3);
			//musi byt neterminal na vrchole zasobnika
			if(hhelp3.indexibus != NETERM){
				error = SYNTAX_ERR;
        clearAll();
        return error;
			}

			if(stackEmpty(stack2)){
        error = SYNTAX_ERR;
        clearAll();
        return error;
      }
			//na vrchole musela byt prava zatvorka a zasobnik musel skoncit prazdny
			stackTopPop(stack2,&hhelp4);
			if((hhelp4.indexibus == PZATV) && (stackEmpty(stack2))){
				//zredukujem na E
				stackPop(stack1);	//odstranim L_HANDLE

				neterminal = hhelp3;	//hodnota z neterminalu z hhelp3
				neterminal.indexibus = NETERM;
				stackPush(stack1,&neterminal);
        error = SUCCESS;
				return error;
			}

			//ak sa dostane sem tak je syntakticka chyba
			error = SYNTAX_ERR;
			clearAll();
			return error;
		}else{
			error = SYNTAX_ERR;
      errorFce();
      clearAll();
      return error;
		}		//pre pravu zatvorku
				//pre neterminal
	}

	error = SYNTAX_ERR;
	return error;
}

int expresion_parser()
{
   int bracket_counter = 0;
	//front_token();
	/// TODO treba po nasadanie zmazat get_token - sluzi len na testovanie
	//get_token();
	//oba zasobniky
	tStack Stack1;
	stackInit(&Stack1);  //zasobnik pre terminaly
	tStack Stack2;
	stackInit(&Stack2);	//zasobnik na odkladanie

	//ci mam citat dalsi token alebo nie
	bool readToken = true;

	//pravy index;
	SAData right_index, left_index;
	SAData pom;
	SAData pom1;
	SAData vyber;
	SAData end;	//
	SAData top;
	pom.sym_data.name = NULL;
	pom.indexibus = DOLAR;

	stackPush(&Stack1,&pom);
	//pushnuty dolar na prvom zasobniku

	do{
//	for(int i = 0; i<6;i++){

		if(readToken){	//ziskam si index do tabulky symbolov podla typu tokenu
			catch_index(&right_index,&bracket_counter);
      if(error != SUCCESS)
        return error;
		}

		stackTop(&Stack1,&left_index);		//do left_index si dam vrchol zasobniku


		//neterminaly preskakujem aby som ziskal najblizsi terminal na porovnanie do prec.t
		while(left_index.indexibus > DOLAR){
			stackTopPop(&Stack1,&vyber);
            stackPush(&Stack2,&vyber);
            stackTop(&Stack1,&left_index); 	//na leftIndex mam prvy terminal, ktory budem porovnavat s rightindex
		}

		//vratim spat vsetko z druheho zasobniku
		while(!stackEmpty(&Stack2)){
			stackTopPop(&Stack2,&vyber);
            stackPush(&Stack1,&vyber);
		}

		//kontrola konca volania funkcie
		if(bracket_counter == -1){
			right_index.indexibus = DOLAR;
		}

		switch (precedense_table[left_index.indexibus][right_index.indexibus]){	//case by takto fakci uz "len" doplnit :D
			case '=':
				readToken = true;
				stackPush(&Stack1,&right_index);
				break;
			case '<':
				readToken = true;
											//zatial bez druheho stacku
				stackTop(&Stack1,&pom1);
				while((pom1.indexibus > DOLAR)){
						stackTopPop(&Stack1,&vyber);
						stackPush(&Stack2,&vyber);
						stackTop(&Stack1,&pom1);
				}

				//vkladany "<" na zasobnik
				pom.indexibus = L_HANDLE;
				//vlozim "<" do zasobniku
				stackPush(&Stack1,&pom);

				//vlozim odlozene neterminaly na stack
				while(!stackEmpty(&Stack2)){
					stackTopPop(&Stack2,&vyber);
					stackPush(&Stack1,&vyber);
				}

				//vlozim terminal na zasobnik
				stackPush(&Stack1,&right_index);


				break;
			case '>':
				reduction(&Stack1,&Stack2);
        if(error != SUCCESS)
            //TODO ak nahodou bude chyba tak tu nevymazavam memory
            return error;
				//ak by bolo nieco zle tak sa sem nedostane;
				readToken = false;
				break;
			case 'E':
				printf("nastala chyba indexovanie do preced.table\n");
				error = SYNTAX_ERR;
				clearAll();
        return error;
				break;
		}

		stackTop(&Stack1,&top);
		 while(top.indexibus > DOLAR){
            stackTopPop(&Stack1,&vyber);
            stackPush(&Stack2,&vyber);
            stackTop(&Stack1,&top);
        }

		stackTop(&Stack1,&end);
        while(!stackEmpty(&Stack2)){
            stackTopPop(&Stack2,&vyber);
            stackPush(&Stack1,&vyber);
        }


		if(readToken){
//			printf("token: |%s|\n",token.data);
			front_token();
			if(error != SUCCESS){
				//errorFce();
				clearAll();
        return error;
			}
		}


//	}
	}while(!((right_index.indexibus == DOLAR) && (end.indexibus == DOLAR)));
//	show_stacks2(&Stack1,&Stack2);



return error;
}
