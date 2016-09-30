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
  **//*
static bool test_key_words()
{
    for( int i = 0; i< 2; i++){
        if((strcmp(token.data,key_words[i]) == 0)){
            return true;
        }
    }
    return false;
}*/

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
	token.error = E_LEXICAL;
}


static void extend_token(int *i, char c)
{
	token.data = (char*)realloc(token.data,(*i)*sizeof(char) + 2);
//	if(token.data == NULL)
//		return NULL;
	token.data[(*i)+1] = '\0';
	token.data[(*i)]=c;
	(*i)++;
//	return token.data;
}

/**
  * Identifikuje jednotlive lexemy a vracia Token
  * TODO
  * 	
  *	doplnit stavy
  *	je zlozeny identifikator rozdeleny na Class a ID?
  */
Ttoken get_token(){
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
	while(((c= getc(file)) != EOF) && (end_cycle)){
		switch (stav)
		{
		case S_START:
			{
				if(isspace(c)){
					printf("[S_START -if]  \t\t---%c---\n",c);
				    stav = S_START;
					break;
			    }else if( (c == '$') || (c == '_') || (isalpha(c))){
					printf("[S_START -alpha] \t---%c---\n",c);	
					stav = S_ID;
				}
				else if( isdigit(c)){
                    printf("[S_START -digit] \t---%c---\n",c);
					stav = S_INT;		     
                }
				else{
					printf("[S_START -else] \t---%c---\n",c);
					stav = S_START;
					break;		
				}			
				return_char(c);
        		break;
		    }
		case S_ID:
			{
				if((c == '$') || (c == '_') || (isalpha(c)) || (isdigit(c))){
					printf("[S_JID -if]   \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_ID;
					token.stav = S_ID;	
				}
				else{
					printf("[S_JID -else]   \t---%c---\n",c);
					return_char(c);
					stav=S_END;
				}
				break;
			}
		case S_MULTI_ID:
        case S_INT:
			{
				if(isdigit(c)){
					printf("[S_INT] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_INT;
					token.stav = S_INT;
				}else if( c == '.'){
					printf("[S_INT .] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_DOUBLE;
					token.stav = S_DOUBLE;					
				}else if( (c == 'e') || c == 'E'){
					printf("[S_INT e] \t\t---%c---\n",c);
					//treba dorobit uz sa mi nechce
				}
				else{
					printf("[S_INT else] \t\t---%c---\n",c);
					return_char(c);
					stav=S_END;
				}
				break;
			}
		case S_DOUBLE:
			{
				if(isdigit(c)){
					printf("[S_DOUBLE] \t\t---%c---\n",c);
					extend_token(&i,c);
					stav = S_DOUBLE;
					token.stav = S_DOUBLE;
					token.error = E_OK;
				}else{
					printf("[S_DOUBLE NE] \t\t---%c---\n",c);
					stav = S_END;
					token.error = E_LEXICAL;
					return_char(c);
				}
				break;
			}
        case S_ERROR:
        case S_END:
			{
			printf("[S_END]   \t\t---%c---\n",c);
			if(isspace(c))
				break;
			return_char(c);		//akoby mi nechcelo vracat znak -
			return_char(c);		//preto musim dva krat zavolat return_char
			end_cycle = false;
            break;
        	}
		}
	}


	printf("<===================KONIEC=================>\n");
	return token;			
}
