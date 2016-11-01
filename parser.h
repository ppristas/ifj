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

   int is_build_function();
   int parser();
   int prog();
   int class();
   int after_class();
   int params_after();
   int main_body();
   int build_print();
   int print_params();
   int build_function_call(int decider);

   /*void expand(tStack *p_stack, int num,...); */

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
