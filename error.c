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
	case SUCCESS:
        break;
	case LEXICAL_ERR:
		    fprintf(stderr,"%s :%d:%d:	Chyba v ramci lexikalnej analyzy.\n",filename,token.line,token.column);
		    break;	
	case SYNTAX_ERR:
        fprintf(stderr,"%s :%d:%d:	Chyba v ramci syntaktickej analyzy.\n",filename,token.line,token.column);
        break;
    case SEMANTIC_PROG_ERR:
        fprintf(stderr,"%s :%d:%d:	Semanticka chyba v programe – nedefinovana trida/funkcia/premenna.\n",filename,token.line,token.column);
        break;
    case SEMANTIC_TYPE_ERR:
        fprintf(stderr,"%s :%d:%d:	Semanticka chyba – chyba typovej kompatibility v aritmetickych, retazcovych a relacnych vyrazoch.\n",filename,token.line,token.column);
        break;
    case SEMANTIC_ERR:
        fprintf(stderr,"%s :%d:%d:	Semanticka chyba – nedefinovana.\n",filename,token.line,token.column);
        break;
    case RUNTIME_LOAD_ERR:
        fprintf(stderr,"%s :%d:%d:	Runtime chyba – Chyba pri nacitani ciselnej hodnoty zo vstupu.\n",filename,token.line,token.column);
        break;
    case RUNTIME_INIT_ERR:
        fprintf(stderr,"%s :%d:%d:	Runtime chyba – Chyba pri praci s neinicializovanou premennou.\n",filename,token.line,token.column);
        break;
    case RUNTIME_ZERO_DIV_ERR:
        fprintf(stderr,"%s :%d:%d:	Runtime chyba – Delenie nulou.\n",filename,token.line,token.column);
        break;
    case RUNTIME_ERR:
        fprintf(stderr,"%s :%d:%d:	Runtime chyba – nedefinovana.\n",filename,token.line,token.column);
        break;
    case INTERNAL_ERR:
        fprintf(stderr,"%s :%d:%d:	Interna chyba – chyba alokacie pamate,chyba pri otvarani suboru...\n",filename,token.line,token.column);
        break;
    }
}
