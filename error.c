/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          error.c                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/

#include "error.h"
#include "scaner.h"

//int column = 1;
//int line = 1;

void errorFce()
{
	switch (error)
	{
		case LEXICAL_ERR:
			{
			printf("%s :%d:%d:	Lexical error\n",filename,token.line,token.column);
			break;
			}
		case SYNTAX_ERR:
			{
			}
		case SEMANTIC_PROG_ERR:
			{
			break;
			}
		case SEMANTIC_TYPE_ERR:
		case SEMANTIC_ERR:
		case RUNTIME_ZERO_DIV_ERR:
		case RUNTIME_LOAD_ERR:
		case RUNTIME_INIT_ERR:
		case RUNTIME_ERR:
		case INTERNAL_ERR:
		case SUCCESS:
		break;
	}
}
