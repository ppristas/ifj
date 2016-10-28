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

   #include "stack.h"

   int parser();
   int prog();
   int class();
   int after_class();
   int params_after();
   int main_body();

   void expand(tStack *p_stack, int num,...);

   typedef enum{  
      T_dolar,      // 0 - $ 
      T_EOF,        // 1
      T_smcln,      // 2 semicolon ; 
      N_P,          // 3 Program- vstupny N
      N_C,          // 4 Class
      T_class,      // 5 klucove slovo class
      T_id,         // 6 identifikator
      T_L_KOSZ,     // 7 lava kosata zatvorka
      T_P_KOSZ,     // 8 prava kosata zatvorka
      N_AC,         // 9 after class
      N_SD,         // 10 static declare 
      N_SDA,        // 11 stat declare after
      T_static,     // 12 klucove slovo static
      N_type,       // 13 neterminal typ
      T_double,     // 14 double
      T_String,     // 15 String
      T_int,        // 16 integer
      T_rov,        // 17 rovnatko =
      N_pars,       // 18 params
      N_decl,       // 19 declaration
      N_PA,         // 20 params after
      N_MB,         // 21 main body
      N_NP,         // 22 next param
      N_SList,      // 23 statement list
      N_expr,       // 24 expr
      N_FC,         // 25 function call
      N_VD,         // 26 var declare
      N_FP,         // 27 func params
      N_NE,         // 28 next E
      N_IB,         // 29 in body
      N_Ext,        // 30 extension
      N_RV,         // 31 return value
      T_while,      // 32 while
      T_if,         // 33 if
      T_else,       // 34 else
      T_ret,        // 35 return
      T_LZAT,       // 36 lava zatvorka
      T_PZAT,       // 37 prava zatvorka
      T_void,       // 38 void
}TGrammar;
