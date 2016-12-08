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
   //#include "cleaner.h"
   #include "stack.h"
   //#include "ial.h"
   #include "ilist.h"
   #include "scaner.h"
   #include "temp_tab.h"
   #include "parser.h"
   unsigned int name = 0;
   TMP_HTAB* const_table;
   symData *destExpr;
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
  char *endptr = NULL;
  char *str = NULL;
  int intoTableInt;
  double intoTableDouble;
  iSymbol *symbol_pom = NULL;
  locSymbol *locsymbol_pom = NULL;
  Hash_class *class_table_pom = NULL;
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
      //extern clHTable* STable;         // Ukazatel na celu tabulku symbolov.
  /*    extern locTable* local_table;    // Ukazatel na lokalnu tabulku
      extern char* class_part;         // Po zavolani return_class() bude obsahovat cast ID, ktora bola tvorena classom
      extern char* id_part;            // Rovnako po zavolani return_class() ale bude obsahovat cast tvorenu ID
      extern symbolType assSymbol;     // Typ symbola, do ktoreho sa bude priradovat
*/    //locSymbol* loc_symbol_search(locTable* ptrloctable,char *data); //ukazatel na lokalny symbol
      //printf("%s\n",token2.data);
      if(strchr(token2.data, '.')){
        return_class();

        class_table_pom = class_search(STable,class_part);
        if(class_table_pom == NULL){
          error = SEMANTIC_PROG_ERR;
          return error;
        }
        symbol_pom = Htab_search(class_table_pom->ptr, id_part);
        if(symbol_pom == NULL){
          error = SEMANTIC_PROG_ERR;
          return error;
        }
        if(!(symbol_pom->data->init)){
          error = RUNTIME_INIT_ERR;
          return error;
        }
        pom->sym_data = symbol_pom->data;
        pom->nameID = symbol_pom->name;
      }else{

          //Hash_class *ptrclass
          //lokalna tabulka

          if((locsymbol_pom = loc_symbol_search(local_table, token2.data)) == NULL){

              symbol_pom = Htab_search(ptrclass->ptr, token2.data);
              if(symbol_pom == NULL){
                  error = SEMANTIC_PROG_ERR;
                  return error;
              }

          }

          if(symbol_pom != NULL){
              if(!(symbol_pom->data->init)){
                error = RUNTIME_INIT_ERR;
                return error;
              }
              pom->sym_data = symbol_pom->data;
              pom->nameID = symbol_pom->name;
          }
          if( locsymbol_pom != NULL){
              if(!(locsymbol_pom->data->init)){
                error = RUNTIME_INIT_ERR;
                return error;
              }
              pom->sym_data = locsymbol_pom->data;
              pom->nameID = locsymbol_pom->name;

        }
      }


      pom->indexibus = ID;
      //pom->nameID =
      //pom->sym_data =
      break;
    case S_INT:

      pom->indexibus = ID;
      pom->sym_data = mymalloc(sizeof(struct sym_Data));
      if(pom->sym_data == NULL){
        error = INTERNAL_ERR;
        return error;
      }
      pom->sym_data->init = true;
      pom->sym_data->args = NULL;
      pom->sym_data->type = tInt;
      pom->sym_data->instrPtr = NULL;
      pom->sym_data->funcdata_union.offset=-1;
      intoTableInt = pom->sym_data->ptr_union.i = strtol(token2.data,&endptr,10);
      str = mymalloc(sizeof(char)*25);
      sprintf(str,"@int_pom_%u",name++);  //TODO vygenerovat premennu;
      pom->nameID = str;

    //printf("vygenerovane meno:%s\n",pom->nameID);
      tmp_htab_insert(const_table, pom->sym_data->type, &intoTableInt,pom->nameID,-1);
      break;
		case S_DOUBLE:
		case S_EXP:

      //key = vygenerovat sprintfom
      //data->arg_count = 0;
      //data->args = NULL;
      //data->symbolType = tDouble;
      //data->ptr_union.d = atoi(token2.data);

			pom->indexibus = ID;
      pom->sym_data = mymalloc(sizeof(struct sym_Data));
      if(pom->sym_data == NULL){
        error = INTERNAL_ERR;
        return error;
      }
      pom->sym_data->init = true;
      pom->sym_data->args = NULL;
      pom->sym_data->type = tDouble;
      pom->sym_data->instrPtr = NULL;
      pom->sym_data->funcdata_union.offset = -1;
      intoTableDouble = pom->sym_data->ptr_union.d = strtod(token2.data,&endptr);

      str = mymalloc(sizeof(char)*25);
      sprintf(str,"@double_pom_%u",name++);  //TODO vygenerovat premennu;
      pom->nameID = str;

    //  printf("vygenerovane meno:%s\n",pom->nameID);
      tmp_htab_insert(const_table, pom->sym_data->type, &intoTableDouble,pom->nameID,-1);


      //pom->sym_data->data = /priradim si vytvorenu premennu
			//TODO
			//pretypovanie na double
			//vlozit do tabulky symbolov
			//preconvertovat cez atoi
			break;
		case S_STRING:
      pom->indexibus = ID;
      pom->sym_data = mymalloc(sizeof(struct sym_Data));
      if(pom->sym_data == NULL){
        error = INTERNAL_ERR;
        return error;
      }
      pom->sym_data->init = true;
      pom->sym_data->args = NULL;
      pom->sym_data->type = tString;
      pom->sym_data->instrPtr = NULL;
      pom->sym_data->funcdata_union.offset=-1;
      if(token2.data != NULL)
        pom->sym_data->ptr_union.str= token2.data;
      else pom->sym_data->ptr_union.str = '\0';
      str = mymalloc(sizeof(char)*25);
      sprintf(str,"@str_pom_%u",name++);  //TODO vygenerovat premennu;
      pom->nameID = str;
      //printf("vygenerovane meno:%s\n",pom->nameID);
      tmp_htab_insert(const_table, pom->sym_data->type,pom->sym_data->ptr_union.str ,pom->nameID,-1);
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
			//clearAll();
      return error;
			break;
	}
	return pom->indexibus;
}


int reduction(tStack *stack1,tStack *stack2){
	SAData hhelp1;
	SAData hhelp2;
	SAData hhelp3;
	SAData hhelp4;
  SAData pom;
	SAData neterminal;
  pom.nameID = NULL;
  hhelp1.nameID = NULL;
  hhelp2.nameID = NULL;
  hhelp3.nameID = NULL;
  hhelp4.nameID = NULL;
  neterminal.nameID = NULL;
  pom.sym_data = NULL;
  hhelp1.sym_data = NULL;
  hhelp2.sym_data = NULL;
  hhelp3.sym_data = NULL;
  hhelp4.sym_data = NULL;
  neterminal.sym_data = NULL;

  pom.sym_data = mymalloc(sizeof(struct sym_Data));
  if(pom.sym_data == NULL){
    error= INTERNAL_ERR;
  //  clearAll();
    return error;
  }
  neterminal.sym_data = mymalloc(sizeof(struct sym_Data));

  if(neterminal.sym_data == NULL){
    error= INTERNAL_ERR;
  //  clearAll();
    return error;
  }
  eInstrType instruction;
  char *str = NULL;
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
				//clearAll();
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
				//errorFce();
				//clearAll();
				return error;
			}

			 //generovanie instrukcie TODO cez jednoduchy switch (hhelp3.indexibus){ }

			switch(hhelp3.indexibus){
				case PLUS:
          instruction = I_ADD;
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
    //      clearAll();
          return error;
          break;
			}



			if(stackEmpty(stack2)){		//ak je prazdny nastava chyba -> nedocitali sme pravidlo
                error = SYNTAX_ERR;
            //    clearAll();
                return error;
      }
			//mal by byt dalsi neterminal

			stackTopPop(stack2,&hhelp4);


			//mozem redukovat, kedze je prazdny stack2 a bol neterminal na vrchole
			if((hhelp4.indexibus == NETERM) && (stackEmpty(stack2))){
				//redukujem
        stackPop(stack1);
				//TODO vlozit do pomocnej tabulky symbolov
				// a do instrukcnej pasky
				//TODO zistovat ci je string a double?
        if ((instruction == I_SUB)   ||
            (instruction == I_DIV)   ||
            (instruction == I_MUL)) {
                if((hhelp2.sym_data->type == tString) || (hhelp4.sym_data->type == tString)){
                  //clearAll();
                  error = SEMANTIC_TYPE_ERR;
                  return error;
                }
              if((hhelp2.sym_data->type == tInt) || (hhelp4.sym_data->type == tInt)){
                neterminal.sym_data->type = tInt;   //vysledny by mal byt int
                neterminal.sym_data->funcdata_union.offset=-1;
              }else{
                neterminal.sym_data->type = tDouble;  //vysledny by mat byt double
                neterminal.sym_data->funcdata_union.offset=-1;
              }
              str = mymalloc(sizeof(char)*30);
              sprintf(str,"@Gen_Target_VAR_ARIT%u",name++);
              neterminal.nameID = str;
              tmp_htab_insert(const_table, neterminal.sym_data->type,NULL ,neterminal.nameID,-1);
              //TODO instrukcna paska
              generateLastInstruction(instruction, hhelp2.sym_data, hhelp4.sym_data, neterminal.sym_data, currentList);
        }
				//TODO ak je jeden z NETERMINALOV string a operator je +
        else if ((instruction == I_EQ)    ||
                (instruction == I_NOTEQ)  ||
                (instruction == I_LESS)   ||
                (instruction == I_LESSEQ) ||
                (instruction == I_BIGGER) ||
                (instruction == I_BIGGEREQ)) {
            /* code */
            //semanticka kontrola pri porovnaniach
                if((hhelp2.sym_data->type == tString) || (hhelp4.sym_data->type == tString)){
                  //clearAll();
                  error = SEMANTIC_TYPE_ERR;
                  return error;
                }
                str = mymalloc(sizeof(char)*30);
                sprintf(str,"@Gen_Target_VAR_COM%u",name++);
                neterminal.nameID = str;
                neterminal.sym_data->type = tBool;
                neterminal.sym_data->funcdata_union.offset=-1;
                tmp_htab_insert(const_table, neterminal.sym_data->type,NULL ,neterminal.nameID,-1);
                generateLastInstruction(instruction, hhelp2.sym_data, hhelp4.sym_data, neterminal.sym_data, currentList);
                //TODO instrukcna paska
        }else if (instruction == I_ADD){
              //konkatenacia
              if((hhelp2.sym_data->type == tString) || (hhelp4.sym_data->type == tString)){
                  neterminal.sym_data->type = tString;
                  neterminal.sym_data->funcdata_union.offset=-1;
              }
              else{
                  if((hhelp2.sym_data->type == tDouble) || (hhelp4.sym_data->type == tDouble)){

                    neterminal.sym_data->type = tDouble;
                    neterminal.sym_data->funcdata_union.offset=-1;
                  }else{

                    neterminal.sym_data->type = tInt;
                    neterminal.sym_data->funcdata_union.offset=-1;

                  }
              }
              str = mymalloc(sizeof(char)*30);
              sprintf(str,"@Gen_Target_VAR_ADD_%u",name++);
              neterminal.nameID = str;
              tmp_htab_insert(const_table, neterminal.sym_data->type,NULL ,neterminal.nameID,-1);
              //TODO instrukcna paska
              generateLastInstruction(instruction, hhelp2.sym_data, hhelp4.sym_data, neterminal.sym_data, currentList);
        }
				//error = SUCCESS;
				neterminal.indexibus = NETERM;
				stackPush(stack1,&neterminal);
				return error;
			}else{
				error = SYNTAX_ERR;
        //clearAll();
        return error;
      }

		//*****
		//***** E->(E)
		//*****
		}else if(hhelp2.indexibus == LZATV){
			if(stackEmpty(stack2)){
				error = SYNTAX_ERR;
        //clearAll();
        return error;
			}
			stackTopPop(stack2,&hhelp3);
			//musi byt neterminal na vrchole zasobnika
			if(hhelp3.indexibus != NETERM){
				error = SYNTAX_ERR;
        //learAll();
        return error;
			}

			if(stackEmpty(stack2)){
        error = SYNTAX_ERR;
        //clearAll();
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
			//clearAll();
			return error;

		}else{
			error = SYNTAX_ERR;
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
  SAData send;
  top.nameID = NULL;
  end.nameID = NULL;
  vyber.nameID = NULL;
  pom1.nameID = NULL;
  pom.nameID = NULL;

  top.sym_data = NULL;
  end.sym_data = NULL;
  vyber.sym_data = NULL;
  pom1.sym_data = NULL;

	pom.sym_data = NULL;
	pom.indexibus = DOLAR;

  if(const_table == NULL){
    const_table = (TMP_HTAB*)mymalloc(sizeof(TMP_HTAB));
    tmp_htab_init(const_table);
  }
	stackPush(&Stack1,&pom);
	//pushnuty dolar na prvom zasobniku

	do{
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
            return error;
				readToken = false;
				break;
			case 'E':
				error = SYNTAX_ERR;
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
			front_token();
			if(error != SUCCESS){
        return error;
			}
		}

	}while(!((right_index.indexibus == DOLAR) && (end.indexibus == DOLAR)));
  stackTopPop(&Stack1, &send);
  if(priradenie){
    if(send.sym_data->type == assSymbol){
      destExpr = send.sym_data;
      error = SUCCESS;
    }else{
      if (assSymbol == tDouble) {
        if(send.sym_data->type == tString){
          error = SEMANTIC_TYPE_ERR;
        }
      }
      if(assSymbol == tInt){
        if(send.sym_data->type == tString)
          error = SEMANTIC_TYPE_ERR;
        if(send.sym_data->type == tDouble)
          error = SEMANTIC_TYPE_ERR;
      }
      if(send.sym_data->type == tString){
          error = SEMANTIC_TYPE_ERR;
      }
      if(assSymbol == tString){
        if(send.sym_data->type == tInt){
          error = SEMANTIC_TYPE_ERR;
        }
        if(send.sym_data->type == tDouble){
          error = SEMANTIC_TYPE_ERR;
        }
        if(send.sym_data->type == tDouble){
          error = SEMANTIC_TYPE_ERR;
        }
      }
    /* code */
    }
  }
  destExpr = send.sym_data;
  ///printf("---%s----%d--\n\n",send.nameID,send.sym_data->type);
  //Print_table(const_table);
return error;
}
