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


Ttoken token;

/**
  *  Vrati znak do bufferu
  *
  */
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

/*
char *allocate(int i)
{
	char *string;
	if((string = (char*) malloc(sizeof(char)*i + 2)) == NULL)
		return NULL;
	return string;
}*/

static void extend_token(int *i, char c)
{
	token.data = (char*)realloc(token.data,(*i)*sizeof(char)+2);
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
  * 	Zmenit navratovy typ
	doplnit stavy
	je zlozeny identifikator rozdeleny na Class a ID?
  */
void get_token(){
	char c ;
	TStav stav = S_START;
	init_token();
	int i = 0;
/*skusam tu*/
	extend_token(&i,'k');
	if(token.data == NULL){
		exit(1);
	}
	printf("%s string \n\n",token.data);


	
	free(token.data);	
	while((c= getc(file)) != EOF){
		switch (stav)
		{
		case S_START:	//pociatocny stav
			{
			if(isspace(c))				//prazdny znak
			{
				stav=S_START;
				break;
			}
			else if(isalpha(c))	stav = S_JID;	
			else if(isdigit(c))	stav = S_INT;
			else {
				stav = S_START; 
				break;
			}	
			return_char(c);
			break;
			}

		case S_JID:	//Identifikator
			{
			if((isalpha(c)) || (isdigit(c))){
				stav = S_JID;
				printf("%c",c);
			}
			else if( c == '.'){
				stav = S_ZID;
				printf("%c",c);
				break;
			}	
			else{
				printf("\t IDENTIFIKATOR\n");
				return_char(c);
				stav = S_START;	
			}
			break;
			}
		case S_ZID:
			{
				if((isalpha(c)) || (isdigit(c))){
					stav = S_ZID;
					printf("%c",c);
				}else{
					printf("\t ZLOZENY IDENTIFIKATOR\n");
					stav = S_START;
				}
				break;
			}
		case S_INT:	//intiger
			{
			if(isdigit(c)){
				stav = S_INT;
				printf("%c",c);
			}
			else{
				printf("\t INT\n");
				stav = S_START;
				return_char(c);
			}
			break;
			}
		}
	}
	return;
}
