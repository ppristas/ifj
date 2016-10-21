#ifndef ERROR_H
#define ERROR_H


#define SUCCESS 0

// --Chyba v ramci lexikalnej analyzy
//chyba v programu v ramci lexikalni analyzy (chybna struktura aktulniho lexemu).
#define LEXICAL_ERR 1

// --Chyba v ramci syntaktickej analyzy
//chyba v programu v ramci syntakticke analyzy (chybna syntaxe programu).
#define SYNTAX_ERR 2


// --Semanticke chyby--
//semanticka chyba v programu – nedefinovana trida/funkce/promenna, pokus o re-definici tridy/funkce/promenne, atd.
#define SEMANTIC_PROG_ERR 3
/*semanticka chyba typove kompatibility v aritmetickych, retezcovych a relacnich
 *vyrazech, prip. spatny pocet ci typ parametru u volani funkce. */
#define SEMANTIC_TYPE_ERR 4
//ostatni semanticke chyby.
#define SEMANTIC_ERR 6


// --Chyby za behu--
//behova chyba pri nacitani ciselne hodnoty ze vstupu.
#define RUNTIME_LOAD_ERR 7
//behova chyba pri praci s neinicializovanou promennou.
#define RUNTIME_INIT_ERR 8
//behova chyba deleni nulou.
#define RUNTIME_ZERO_DIV_ERR 9
//ostatni behove chyby.
#define RUNTIME_ERR 10

// --Interna chyba--
/*interni chyba interpretu tj. neovlivnena vstupnim programem (napr. chyba
 *alokace pameti, chyba pri otvirani souboru s ridicim programem, spatne parametry
 *prikazove radky atd.).*/
#define INTERNAL_ERR 99

// globalna premenna pre navratovu hodnotu chyby
int error_ret;
int printError();

#endif