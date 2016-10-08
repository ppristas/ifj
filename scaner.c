/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          scaner.c                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/


#include "scaner.h"
#include <stdbool.h>


//pocet klucovych slov
#define	num_key_words  17

//globalna deklaracia struktury Ttoken
Ttoken token;
Enum_error error;

//klucove slova
char *key_words[num_key_words] = {	"boolean\0",
					"break\0",
					"class\0",
					"continue\0",
					"do\0",
					"double\0",
					"else\0",
					"false\0",
					"for\0",
					"if\0",
					"int\0",
					"return\0",
					"String\0",
					"static\0",
					"true\0",
					"void\0",	
					"while\0",};

/**
  * Funkcia na overenie ci je token klucove slovo 
  * navratovy typ : boolean
  **/
static bool test_key_words(char *word)
{
    for( int i = 0; i<num_key_words; i++){
        if((strcmp(word,key_words[i]) == 0)){
            return true;
        }
    }
    return false;
}

/**
  *  Vrati znak do bufferu
  *  
  **/
static void return_char(char c)
{
    // vratime znak ak je neprazdny
    if (!isspace(c))
        ungetc(c, file);

}

/**
  *inicializacia tokenu
  */
static void init_token()
{
	token.data = NULL;
	token.stav = S_START;
}


static void extend_token(int *i, char c)
{
	token.data = (char*)realloc(token.data,(*i)*(sizeof(char)) + 2);
//	if(token.data == NULL)
//		return NULL;
	token.data[(*i)+1] = '\0';
	token.data[(*i)]=c;
	(*i)++;
//	return token.data;
}

static void fill_token(TStav status,Enum_error err)
{
	token.stav = status;
	error = err;
}

/**
  * Identifikuje jednotlive lexemy a vracia Token
  * TODO
  * 	
  *	doplnit stavy
  *	je zlozeny identifikator rozdeleny na Class a ID?
  */
Ttoken get_token(){
	error = E_OK;
	char c ;
	TStav stav = S_START;

	int i = 0;
	bool end_cycle = true;
/**************/



	if(token.data != NULL){
		free(token.data);	
	}
	init_token();	

/*skusam tu*/
	while((c= getc(file)) && (end_cycle)){
		switch (stav)
		{
		case S_START:
			{
				if(isspace(c)){
					//printf("[S_START -if]  \t\t---%c---\n",c);
				    stav = S_START;
					break;
			    }else if( (c == '$') || (c == '_') || (isalpha(c))){
					//printf("[S_START -alpha] \t---%c---\n",c);	
					stav = S_ID;
				}
				else if( isdigit(c)){
                  //  printf("[S_START -digit] \t---%c---\n",c);
					stav = S_INT;		     
                }
				else if(c == '+')		stav = S_PLUS;
				else if(c == '-')		stav = S_MINUS;
				else if(c == '*')		stav = S_MULTI;
				else if(c == '/'){		stav = S_DIV;break;}
				else if(c == ';')		stav = S_SEMICOLON;
				else if(c == ',')		stav = S_CIARKA;
				else if(c == '='){		stav = S_PRIR;extend_token(&i,c);break;}
				else if(c == EOF)		stav = S_EOF;
				else if(c == '<'){		stav = S_MEN; extend_token(&i,c);break;}
				else if(c == '>'){		stav = S_VAC; extend_token(&i,c);break;}
				else{
				//	printf("[S_START -else] \t---%c---\n",c);
					stav = S_END;
					break;		
				}
				return_char(c);			
				//extend_token(&i,c);
        		break;
		    }
		case S_ID:
			{
				if((c == '$') || (c == '_') || (isalpha(c)) || (isdigit(c))){
				//	printf("[S_JID -if]   \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_ID;
					fill_token(stav,E_OK);	
				}//doplnit pre zlozeny identifikator s bodkou .
				else{
				//	printf("[S_JID -else]   \t---%c---\n",c);
					return_char(c);
					if(test_key_words(token.data)){
						stav = S_KEY;
				//		printf("vyznam\n");
						fill_token(stav,E_OK);
					}else{
					stav = S_END;
					fill_token(S_ID,E_OK);
					}
				}
				break;
			}
		case S_MULTI_ID: ////rozdelit class.ID na tri tokeny alebo nie?
			{
			}	
        case S_INT:
			{
				if(isdigit(c)){
				//	printf("[S_INT] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_INT;
					//fill_token(S_INT,E_OK);
				}else if( c == '.'){
				//	printf("[S_INT .] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_DOUBLE_POM;
					//fill_token(S_DOUBLE,E_OK);				
				}else if( (c == 'e') || c == 'E'){
				//	printf("[S_INT e] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_EXP;
					break;
					//fill_token(stav,E_OK);
				}else if((isalpha(c)) || ( c == '$') || ( c == '_')){
				//	printf("CHYBA INT\n");
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
				}
				else{
				//	printf("[S_INT else] \t\t---%c---\n",c);
					return_char(c);
					fill_token(stav,E_OK);
					stav = S_END;
				}
				break;
			}
		case S_DOUBLE_POM:				//cislo double 1.222554
			{
				if(isdigit(c)){
					printf("[S_DOUBLE] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_DOUBLE;
					fill_token(stav,E_OK);					
				} //******TODO doplnit pre exponent
				/*else if( (c == 'e') || (c == 'E')){
					printf("[S_DOUBLE e] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_EXP;
					fill_token(stav,E_OK);		
				}*/
				else{
				//	printf("[S_DOUBLE NE] \t\t---%c---\n",c);
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
					return_char(c);
				}
				break;
			}
		case S_DOUBLE:
			{
				if(isdigit(c)){
				//	printf("[S_DOUBLE] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_DOUBLE;
				}
				else if( (c == 'e') || (c == 'E')){
              //      printf("[S_DOUBLE e] \t\t---%c---\n",c);
                    extend_token(&i,c);
                    stav = S_EXP;
					break;
                    //fill_token(stav,E_OK);     
                }else if((isalpha(c)) || ( c == '$') || ( c == '_') || (c== '.')){
			//		printf("CHYBA DOUBLE\n");
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
					return_char(c);
				}else{
			//		printf("[S_DOUBLE DONE] \t\t---%c---\n",c);
					fill_token(stav,E_OK);
					stav = S_END;
					return_char(c);
				}
				break;
			}
		case S_EXP:					//zakladny exponent bez znamienka 2e120
			{
				if(isdigit(c)){
			//		printf("[S_EXP num] \t\t---%c---\n",c);
					stav = S_EX;
					fill_token(stav,E_OK);
					extend_token(&i,c);
				} //doplnit if pre znamienka
				else if(( c == '+') || ( c == '-')){
			//		printf("[S_EXP SIGN] \t\t---%c---\n",c);
					stav = S_EXP_SIGNED;
					fill_token(stav,E_OK);
					extend_token(&i,c);
				}else{
			//		printf("[S_EXP else] \t\t---%c---\n",c);
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
					return_char(c);					
				}
				break;
					
			}
		case S_EXP_SIGNED:			//exponent so znamienkami 2e+4566
			{
				if(isdigit(c)){
			//		printf("[S_EXPS num] \t\t---%c---\n",c);
					stav = S_EX;
					fill_token(stav,E_OK);
					extend_token(&i,c);
				}else{
				//ako mam detekovat ze to je lexikalna chyba
				//vzdy pri tomto "else" mi nasledne vrati nulu pretoze
				// to precita iny znak ako cislo aj ked tam bude +,- atd...
			//		printf("[chyba exp_signed]\n");
					stav = S_ERROR;
					return_char(c);
				}
				//return_char(c);
				break;
				
			}
		case S_EX:
			{
				if(isdigit(c)){
			//		printf("[S_EX] \t\t---%c---\n",c);
					stav = S_EX;
					extend_token(&i,c);
				}else if( (isalpha(c)) || ( c == '$') || ( c == '_')){
			//		printf("[S_EX CHYBA]\n");
					stav = S_ERROR;
				}
				break;
				
			}
		case S_DIV:
			{
				if(c == '/'){
					stav = S_COM_LINE;
				}else if(c == '*'){
					stav = S_MULTI_COM;
				}else{
					extend_token(&i,'/');
					fill_token(stav,E_OK);
					stav = S_END;
					return_char(c);
				}
				break;
			}
		case S_COM_LINE:
			{
				if(c == '\n'){
					stav = S_START;
				}else if( c == EOF)
					stav = S_EOF;
				else
					stav = S_COM_LINE;
				break;
			}
		case S_MULTI_COM:
			{
		//		printf("multiline stav\n");
				if(c == '*'){
					c = getc(file);
					//printf("cococo %c\n",k);
					if( c == '/'){
						stav = S_START;
						i = 0;
					}
				}else if(c == EOF){
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
					
				}else{
					stav = S_MULTI_COM;
				}
				break;
			}
		case S_PRIR:
			{
				//printf("znak -<%c>-\n",c);
				if(c == '='){
					//printf("kokosajs %s\n",token.data);
					extend_token(&i,c);	
					//printf("dalsiznak >%s<\n",token.data); 
					stav = S_ROVNY;
					fill_token(stav,E_OK);
					return_char(c);
				}else{
					//extend_token(&i,'=');
					fill_token(stav,E_OK);
					stav = S_END;
					return_char(c);
				}
				break;
			}
		case S_MEN:
			{
				if(c == '='){
					extend_token(&i,c);
					stav = S_MENROV;
					fill_token(stav,E_OK);
					return_char(c);
				}else{
					fill_token(stav,E_OK);
					stav = S_END;
					return_char(c);
				}
				break;
			}
		case S_VAC:
			{
				if(c == '='){
					extend_token(&i,c);
					stav = S_VACROV;
					fill_token(stav,E_OK);
					return_char(c);
				}else{
					fill_token(stav,E_OK);
					stav = S_END;
					return_char(c);
				}
				break;
			}
        case S_ERROR:
			{
				fill_token(stav,E_LEXICAL);
				error = E_LEXICAL;
				end_cycle = false;
				break;
			}
		case S_PLUS:
		case S_MINUS:
		case S_MULTI:
		case S_EOF:
		case S_SEMICOLON:
		case S_CIARKA:
			{
				extend_token(&i,c);
				fill_token(stav,E_OK);
			}
		case S_ROVNY:
		case S_KEY:
		case S_MENROV:
		case S_VACROV:
			{
				end_cycle = false;
				break;
			}
        case S_END:
			{
		//	fill_token(stav,E_OK);
		//	printf("[S_END]   \t\t---%c---\n",c);
			if(isspace(c))
				break;
			return_char(c);		//akoby mi nechcelo vracat znak 
								//(ak sa nudite mozte to nejak vycihat :D
			return_char(c);		//preto musim dva krat zavolat return_char
			end_cycle = false;
            break;
        	}
		}
	}


	printf("<==============KONIEC VOLANIA FUNKCIE=============>\n");
	return token;			
}
