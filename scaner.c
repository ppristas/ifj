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
#include "error.h"

//pocet klucovych slov
#define	num_key_words  17

//globalna deklaracia struktury Ttoken
Ttoken token;


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
static void test_key_words(char *word)
{
    for( int i = 0; i<num_key_words; i++){
        if((strcmp(word,key_words[i]) == 0)){
            token.stav = S_KEY;
			return;
        }
    }
	token.stav = S_ID;
    return;
}

/**
  *  Vrati znak do bufferu
  *  
  **/
static void return_char(int c)
{
    // vratime znak ak je neprazdny
    if(!isspace(c))    
		c = ungetc(c, file);

	if(isprint(c))
		column--;
}

/**
  *inicializacia tokenu
  */
static void init_token()
{
	token.data = NULL;
	token.stav = S_START;
	token.column = column;
	token.line = line;
}


static void extend_token(int *i, int c)
{

	if(token.data == NULL)
		token.data = malloc(sizeof(char) +2);
	else
		token.data = (char*)realloc(token.data,(*i)+2);
	if(token.data == NULL){
		error = INTERNAL_ERR;
		return;
	}
	token.data[(*i) +1] = '\0';
	token.data[(*i)]=c;
	(*i)++;
	
//	return token.data;
}

static void fill_token(TStav status,int err)
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
	
	error = SUCCESS;
	int c = getc(file);;
	TStav stav = S_START;
	
	int i = 0;
	bool end_cycle = true;

	if(token.data != NULL){
		free(token.data);	
	}
	init_token();	

	do{
		switch (stav)
		{
		case S_START:
			{
				token.column = column;
				token.line = line;
				if(isspace(c)){
				    stav = S_START;
					break;
			    }else if( (c == '$') || (c == '_') || (isalpha(c))){	
					stav = S_ID;
				}
				else if( isdigit(c)){ 
					stav = S_INT;		     
				}
				else if(c == '+')		stav = S_PLUS;//extend_token(&i,c);c = ungetc(c,file);column++;break;}
				else if(c == '-')		stav = S_MINUS;
				else if(c == '*')		stav = S_MULTI;
				else if(c == '/'){		stav = S_DIV;break;}
				else if(c == ';')		stav = S_SEMICOLON;
				else if(c == ',')		stav = S_CIARKA;
				else if(c == '=')		stav = S_PRIR;
				else if(c == EOF)		stav = S_EOF;
				else if(c == '<')		stav = S_MEN;
				else if(c == '>')		stav = S_VAC;
				else if(c == '(')		stav = S_LZAT;
				else if(c == ')')		stav = S_PZAT;
				else if(c == '{')		stav = S_L_KOSZ;
				else if(c == '}')		stav = S_P_KOSZ;
				else if(c == '!')		stav = S_VYKR;
				else if(c == '"'){		stav = S_STRING;break;}
				else{
					extend_token(&i,c);
					fill_token(S_ERROR,LEXICAL_ERR);
					stav = S_ERROR;
					break;		
				}
//				printf("((((%c))))\n",c);
				extend_token(&i,c);		
        		break;
		    }
		case S_ID:
			{
				if((c == '$') || (c == '_') || (isalpha(c)) || (isdigit(c))){
					stav = S_ID;
					extend_token(&i,c);
						
				}//doplnit pre zlozeny identifikator s bodkou .TODO
				else if( c == '.'){
					fill_token(S_ID,SUCCESS);
					//ungetc(c,file);
					//column--;
					extend_token(&i,c);
					stav = S_CLASS;
				}
				else{
//					printf("koniec slova: -%c-\n",c);
					fill_token(stav,SUCCESS);
					test_key_words(token.data);	
					stav = S_END;
					ungetc(c,file);
					column--;
					return token;
				}

				break;
			}
	case S_CLASS: ////rozdelit class.ID na tri tokeny alebo nie?
			{
				if((isalpha(c)) || (c == '$') || (c == '_')){
					stav = S_CLASS;
					extend_token(&i,c);
				}else{
					stav = S_END;
					ungetc(c,file);
					column--;
					return token;
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
					fill_token(stav,LEXICAL_ERR);
					c = ungetc(c,file);
					column--;
				}
				else{
					c = ungetc(c,file);
					column--;
					fill_token(stav,SUCCESS);
					return token;
				}
				break;
			}
		case S_DOUBLE_POM:				//cislo double 1.222554
			{
				if(isdigit(c)){

					extend_token(&i,c);
					stav = S_DOUBLE;
					fill_token(stav,SUCCESS);					
				} //******TODO doplnit pre exponent
				else if( (c == 'e') || (c == 'E')){

					extend_token(&i,c);
					stav = S_EXP;
					fill_token(stav,SUCCESS);		
				}
				else{
					stav = S_ERROR;
					fill_token(stav,LEXICAL_ERR);
					c = ungetc(c,file);
					column--;
				
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
					fill_token(stav,LEXICAL_ERR);
					while((c = getc(file))){
                        if(isspace(c)){
                            break;
                        }
                        column++;
                    }
                    c = ungetc(c,file);
                    column--;
					
				}else{
					fill_token(stav,SUCCESS);
					stav = S_END;
					c=ungetc(c,file);
					column--;
					return token;
				}
				break;
			}
		case S_EXP:					//zakladny exponent bez znamienka 2e120
			{
				if(isdigit(c)){
					stav = S_EX;
					fill_token(stav,SUCCESS);
					extend_token(&i,c);
				} //doplnit if pre znamienka
				else if(( c == '+') || ( c == '-')){
					stav = S_EXP_SIGNED;
					fill_token(stav,SUCCESS);
					extend_token(&i,c);
				}else{
					stav = S_ERROR;
					fill_token(stav,LEXICAL_ERR);
					while((c = getc(file))){
                        if(isspace(c)){
                            break;
                        }
                        column++;
                    }
                    c = ungetc(c,file);
                    column--;

				
				}
				break;
					
			}
		case S_EXP_SIGNED:			//exponent so znamienkami 2e+4566
			{
				if(isdigit(c)){
					stav = S_EX;
					fill_token(stav,SUCCESS);
					extend_token(&i,c);
				}else{
				//ako mam detekovat ze to je lexikalna chyba
				//vzdy pri tomto "else" mi nasledne vrati nulu pretoze
				// to precita iny znak ako cislo aj ked tam bude +,- atd...
			//		printf("[chyba exp_signed]\n");
					stav = S_ERROR;
					fill_token(stav,LEXICAL_ERR);
					while((c = getc(file))){
						if(isspace(c)){
							break;
						}
						column++;
					}
					c = ungetc(c,file);
					column--;
					//c = ungetc(c,file);
					//column--;
				
				}
				//return_char((char)c);
				break;
				
			}
		case S_EX:
			{
				if(isdigit(c)){
					stav = S_EX;
					extend_token(&i,c);
				}else if( (isalpha(c)) || ( c == '$') || ( c == '_')){
					stav = S_ERROR;
					fill_token(stav,LEXICAL_ERR);
					while((c = getc(file))){
                        if(isspace(c)){
                            break;
                        }
                        column++;
                    }
                    c = ungetc(c,file);
                    column--;

				}else{
					fill_token(stav,SUCCESS);
					c = ungetc(c,file);
					column--;
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
					fill_token(stav,SUCCESS);
					stav = S_END;
					c = ungetc(c,file);
					column--;
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
					fill_token(stav,LEXICAL_ERR);
					
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
					fill_token(stav,SUCCESS);
					c = ungetc(c,file);
					column--;
				}else{
					fill_token(stav,SUCCESS);
					stav = S_END;
					c = ungetc(c,file);
					return token;	
				}
				break;
			}
		case S_MEN:
			{
				if(c == '='){
					extend_token(&i,c);
					stav = S_MENROV;
					fill_token(stav,SUCCESS);
				}else{
					fill_token(stav,SUCCESS);
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
					fill_token(stav,SUCCESS);
				//	return_char((char)c);
				}else{
					fill_token(stav,SUCCESS);
					stav = S_END;
					c = ungetc(c,file);
					column--;
				}
				break;
			}
		case S_VYKR:
			{
				if(c == '='){
					stav = S_NEROV;
					extend_token(&i,c);
					fill_token(stav,SUCCESS);
				}else{
					stav = S_ERROR;
					return_char((char)c);
				}
				break;
	
			}
        case S_ERROR:
			{
				c = ungetc(c,file);
				column--;
				end_cycle = false;
				break;
			}
		case S_STRING:
			{
				if( c == '"'){
					fill_token(stav,SUCCESS);
					stav = S_END;
					return token;
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
				if((c >= 48) && ( c <= 51)){
					pom[0] = c;
					c = getc(file);
					if((isdigit(c)) && ((c >= 48) && (c <= 55))){
						pom[1] = c;
						c = getc(file);
	                    if((isdigit(c)) && ((c >= 49) && (c <= 55))){
							pom[2] = c;
							pom[3] = '\0';
							oktal_num = strtol(pom,NULL,8);
							c = oktal_num;
							if(c <= 377){
								extend_token(&i,(char)c);
								stav = S_STRING;	
							}else{
								while((c = getc(file)) != '"'){
				                    if(c == EOF)
            				            break;               				
								}						
								fill_token(S_ERROR,LEXICAL_ERR);
								stav =  S_ERROR;
							}
							break;
						}
					}
				}

					while((c = getc(file)) != '"'){
                        if(c == EOF)
                            break;
					}
					stav = S_ERROR; 		
					fill_token(S_ERROR,LEXICAL_ERR);		
				//}else{
				//	fill_token(S_ERROR,E_L_ESCAPE);
				//	stav = S_ERROR;
				//}// 
			}else{
				while((c = getc(file)) != '"'){
					if(c == EOF){
						break;
					}
				}

				fill_token(S_ERROR,LEXICAL_ERR);
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
		case S_NEROV:
		case S_ROVNY:
		case S_MENROV:
		case S_VACROV:
		case S_KEY:
			{
				fill_token(stav,SUCCESS);
				c = ungetc(c,file);
				return token;
				/*
				printf("---%c---\n",c);
				token.stav = stav;
				error = E_OK;
				ungetc(c,file);
				column--;
				printf(">>>%c<<<<\n",c);
				end_cycle = false;
				break;*/
			}
        case S_END:
			{
//				printf("===%c===\n",c);
				c = ungetc(c,file);
				column--;
				end_cycle = false;
				break;
	    	}
			
		}
		if(error != SUCCESS){
			break;
		}
		
		if( c == '\n'){
			line++;
			column = 1;
		}else if((isprint(c)) || (isspace(c))){
			column++;	
		}
		//printf("column:%d line:%d char:%c\n",line,column,c);
	}while((c = getc(file)) && (end_cycle));


//	printf("<==============KONIEC VOLANIA FUNKCIE=============>\n");
	return token;			
}
