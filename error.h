#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

	#define SUCCESS         0 //bez chyby
	#define ERR_LEXICAL		1 //lexikalna chyba
	#define ERR_SYNTAX 		2 //syntakticka chyba
	#define ERR_NDEF     	3 //nedefinovana premenna/trieda/funkcia pripadne pokus o redefiniciu
	#define ERR_TYPE      	4 //chyba typovej kompatibility
	#define ERR_SEMANTIC  	6 //semanticka chyba
	#define ERR_SNUMBER    	7 //behova chyba nacitania ciselnej hodnoty zo vstupu
	#define ERR_VARNINIT  	8 //behova chyba pri praci s neinicializovanou premennou.
	#define ERR_DIVZERO  	9 //behova chyba pri deleni nulou
	#define ERR_OTHER	  	10 //ostatne behove chyby 
	#define ERR_INTERPRET  	99  //interna chyba interpretera

#endif 