/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          parser.h                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/

   #ifndef PAR_H
   #define PAR_H

   extern clHTable* STable;         // Ukazatel na celu tabulku symbolov.
   extern locTable* local_table;    // Ukazatel na lokalnu tabulku
   extern char* class_part;         // Po zavolani return_class() bude obsahovat cast ID, ktora bola tvorena classom
   extern char* id_part;            // Rovnako po zavolani return_class() ale bude obsahovat cast tvorenu ID
   extern symbolType assSymbol;     // Typ symbola, do ktoreho sa bude priradovat
   extern bool priradenie;          // Sme v priradeni alebo nie 

   int is_build_function();
   int parser();
   int prog();
   int class();
   int after_class();
   int params_after();
   int main_body();
   int main_body_riadiace();
   int build_print();
   int print_params();
   int build_function_call(int decider);

   /*-----------Na druhy priechod-----------*/
   int is_build_function_scnd();
   int prog_scnd();
   int class_scnd();
   int after_class_scnd();
   int params_after_scnd();
   int main_body_scnd();
   int main_body_riadiace_scnd();
   int build_print_scnd();
   int print_params_scnd();
   int build_function_call_scnd(int decider);
   int user_function_call();
   int is_function_call_or_ass();
   int build_in_ID();

   /*------------Ostatne-------------------*/
   int return_class();

   typedef enum{
   F_NbIF,        // 0 not build in function
   F_print,       // 1 print
   F_int,         // 2 readint
   F_double,      // 3 readDouble
   F_string,      // 4 readString
   F_length,      // 5 length
   F_substr,      // 6 substr
   F_compare,     // 7 compare
   F_find,        // 8 compare
   F_sort,        // 9 sort
}BI_Funkce;

   #endif
