/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          string.h                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/
   
#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED


typedef struct {
    char* str;
    int length;
    int allocated;
}string;


int string_init(string *s);
void string_free(string *s);
void string_fill(string *s, char c);
int string_realloc(string *s);
void string_print(string *s);
int string_merge(string *s1, string *s2);
void string_delChar(string *s);
#endif // STRING_H_INCLUDED
