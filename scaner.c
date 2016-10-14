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

	if(token.data != NULL){
		free(token.data);	
	}
	init_token();	

	while((c= getc(file)) && (end_cycle)){
		switch (stav)
		{
		case S_START:
			{
				if(isspace(c)){
				    stav = S_START;
					break;
			    }else if( (c == '$') || (c == '_') || (isalpha(c))){	
					stav = S_ID;
				}
				else if( isdigit(c)){ 
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
				else if(c == '(')		stav = S_LZAT;
				else if(c == ')')		stav = S_PZAT;
				else if(c == '{')		stav = S_L_KOSZ;
				else if(c == '}')		stav = S_P_KOSZ;
				else if(c == '!'){		stav = S_VYKR;extend_token(&i,c);break;}
				else if(c == '"'){		stav = S_STRING;break;}
				else{
					stav = S_ERROR;
					return_char(c);
					break;		
				}
				return_char(c);			
        		break;
		    }
		case S_ID:
			{
				if((c == '$') || (c == '_') || (isalpha(c)) || (isdigit(c))){
					extend_token(&i,c);
					stav = S_ID;
					fill_token(stav,E_OK);	
				}//doplnit pre zlozeny identifikator s bodkou .TODO
				else if( c == '.'){
					stav = S_CLASS;
				}
				else{
				
					if(test_key_words(token.data)){
						stav = S_KEY;
						fill_token(stav,E_OK);
					}else{
					stav = S_END;
					fill_token(S_ID,E_OK);
					}
					return_char(c);
				}
				break;
			}
		case S_CLASS: ////rozdelit class.ID na tri tokeny alebo nie?
			{
				if((isalpha(c)) || (c == '$') || (c == '_')){
					fill_token(stav,E_OK);
					stav = S_END;
					return_char(c);
				}else{
					stav = S_ERROR;
				
				}
				break;
			}	
        case S_INT:
			{
				if(isdigit(c)){
					extend_token(&i,c);
					stav = S_INT;
				}else if( c == '.'){
					extend_token(&i,c);
					stav = S_DOUBLE_POM;				
				}else if( (c == 'e') || c == 'E'){
					extend_token(&i,c);
					stav = S_EXP;
					break;
				}else if((isalpha(c)) || ( c == '$') || ( c == '_')){
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
				}
				else{
					return_char(c);
					fill_token(stav,E_OK);
					stav = S_END;
				}
				break;
			}
		case S_DOUBLE_POM:				//cislo double 1.222554
			{
				if(isdigit(c)){

					extend_token(&i,c);
					stav = S_DOUBLE;
					fill_token(stav,E_OK);					
				} //******TODO doplnit pre exponent
				else if( (c == 'e') || (c == 'E')){

					extend_token(&i,c);
					stav = S_EXP;
					fill_token(stav,E_OK);		
				}
				else{
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
					return_char(c);
				}
				break;
			}
		case S_DOUBLE:
			{
		
				if(isdigit(c)){
					extend_token(&i,c);
					stav = S_DOUBLE;
				}
				else if( (c == 'e') || (c == 'E')){
               
					extend_token(&i,c);
                    stav = S_EXP;
				//	break;
                }else if((isalpha(c)) || ( c == '$') || ( c == '_') || (c== '.')){
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
					return_char(c);
				}else{
					fill_token(stav,E_OK);
					stav = S_END;
					return_char(c);
				}
				break;
			}
		case S_EXP:					//zakladny exponent bez znamienka 2e120
			{
				if(isdigit(c)){
					stav = S_EX;
					fill_token(stav,E_OK);
					extend_token(&i,c);
				} //doplnit if pre znamienka
				else if(( c == '+') || ( c == '-')){
					stav = S_EXP_SIGNED;
					fill_token(stav,E_OK);
					extend_token(&i,c);
				}else{
					stav = S_ERROR;
					fill_token(stav,E_LEXICAL);
					return_char(c);					
				}
				break;
					
			}
		case S_EXP_SIGNED:			//exponent so znamienkami 2e+4566
			{
				if(isdigit(c)){
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
					stav = S_EX;
					extend_token(&i,c);
				}else if( (isalpha(c)) || ( c == '$') || ( c == '_')){
					stav = S_ERROR;
				}else{
					fill_token(stav,E_OK);
					return_char(c);
					stav = S_END;
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
				if(c == '*'){
					c = getc(file);
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
				if(c == '='){
					extend_token(&i,c);	
					stav = S_ROVNY;
					fill_token(stav,E_OK);
					return_char(c);
				}else{
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
				//	return_char(c);
				}else{
					fill_token(stav,E_OK);
					stav = S_END;
					return_char(c);
				}
				break;
			}
		case S_VYKR:
			{
				if(c == '='){
					stav = S_NEROV;
					extend_token(&i,c);
					fill_token(stav,E_OK);
					return_char(c);
				}else{
					stav = S_ERROR;
					return_char(c);
				}
				break;
			}
        case S_ERROR:
			{
				fill_token(stav,E_LEXICAL);
				end_cycle = false;
				break;
			}
		case S_STRING:
			{
				if( c == '"'){
					fill_token(stav,E_OK);
					stav = S_END;
				}
				else if(c == 92){
				//	printf("kekek\n");				/* ak c je \ */
					stav = S_ESCAPE;
				}else if(c == EOF){
					stav = S_ERROR;
				}
				else{
					extend_token(&i,c);
					stav = S_STRING;
				}
			break;
			}
		case S_ESCAPE:
			{
			if(c == 'n'){
				extend_token(&i,'\n');
				stav = S_STRING;
			}else if( c == '\"'){
				extend_token(&i,'\"');
				stav = S_STRING;
			}else if( c == 't'){
				extend_token(&i,'\t');
				stav = S_STRING;
			}else if(c == 92){
				extend_token(&i,'\\');
				stav = S_STRING;
			}else if(isdigit(c)){
				int oktal_num;
				char pom[3];
				if((c >= 48) || ( c <= 51)){
					pom[0] = c;
					c = getc(file);
					if((isdigit(c)) && ((c >= 48) || (c <= 55))){
						pom[1] = c;
						c = getc(file);
	                    if((isdigit(c)) && ((c >= 49) || (c <= 55))){
							pom[2] = c;
							pom[3] = '\0';
							oktal_num = strtol(pom,NULL,8);
					 		c = oktal_num;
							extend_token(&i,c);
							stav = S_STRING;	
							break;
						}
					}
					stav = S_ERROR; 		
					fill_token(S_ERROR,E_ESCAPE);		
				}else{
					fill_token(S_ERROR,E_ESCAPE);
					stav = S_ERROR;
				} 
			}else{
				while((c = getc(file)) != '"'){
					if(c == EOF){
						stav = S_ERROR;
						fill_token(S_ERROR,E_ESCAPE);
						break;
					}
				}

				fill_token(S_ERROR,E_LEXICAL);
				stav = S_ERROR;
			}
			break;
			}
		case S_PLUS:
		case S_MINUS:
		case S_MULTI:
		case S_EOF:
		case S_SEMICOLON:
		case S_CIARKA:
		case S_LZAT:
		case S_PZAT:
		case S_P_KOSZ:
		case S_L_KOSZ:
			{
				extend_token(&i,c);
				fill_token(stav,E_OK);
	//			stav = S_END;
	//			return_char(c);
	//			break;
			}
		case S_NEROV:
		case S_ROVNY:
		case S_MENROV:
		case S_VACROV:
			{
				return_char(c);
				end_cycle = false;
				break;
			}
		case S_KEY:
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
		if(error){
			break;
		}
		
	}


	printf("<==============KONIEC VOLANIA FUNKCIE=============>\n");
	return token;			
}
