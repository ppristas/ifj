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

   int parser();
   int prog();
   int after_class();

   typedef enum{  
      T_EOF,        // 0 
      T_smcln,      // 1 semicolon ; 
      N_P,          // 2 Program- vstupny N
      N_C,          // 3 Class
      T_class,      // 4 klucove slovo class
      T_idc,        // 5 identifikator classy
      T_L_KOSZ,     // 6 prava kosata zatvorka
      T_P_KOSZ,     // 7 lava kosata zatvorka
      N_AC,         // 8 after class
      N_F,          // 9 funckia
      N_DEFS,       // 10 staticka premenna 
      T_static,     // 11 klucove slovo static
      T_type,       // 12 klucove slovo type
      T_id,         // 13 identifikator premennej
      N_I,          // 14 inicializacia
      T_rov,        // 15 rovnatko =
      N_E,          // 16 expression
      N_DEFC,       // 17 definicia lokalnej premennej  
      T_idf,        // 18 identifikator funkcie
      N_AL,         // 19 argument list
      N_AD,         // 20 argument definition
      N_ALE,        // 21 argument list extension
      N_S,          // 22 statement
      T_LZAT,       // 23 lava zatvorka
      T_PZAT,       // 24 prava zatvorka
      T_if,         // 25 if
      T_else,       // 26 else
      T_while,      // 27 while
      T_return,     // 28 return
      /*--------to be continued---------*/
}TGrammar;