/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          main.c                                                  *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/

   #include "parser.h"
   #include "scaner.h"
   #include "error.h"

 int parser()
 {
      get_token();
      if(token.stav == S_EOF)
         error = SYNTAX_ERR;
      error = prog();
      
      if(error != SUCCESS)
         return error;

      return SUCCESS;
 }

 int prog()
 {
      if(error != SUCCESS)
         return error;
      if((token.stav != S_KEY) && (strcmp(token.data,"class")))
         return SYNTAX_ERR;

      get_token();
      if(token.stav != S_ID)
         return SYNTAX_ERR;

      get_token();
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      error = after_class();
      if(error != SUCCESS)
         return error;
      
      get_token();
      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;

      return SUCCESS;
 }

 int after_class()
 {
      return SUCCESS;
 }