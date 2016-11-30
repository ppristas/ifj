/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          parser.c                                                  *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *
   ********************************************************************************/

#include <stdarg.h>
#include <string.h>

#include "stack.h"
#include "parser.h"
#include "cleaner.h"
#include "error.h"
#include "preced.h"
#include "ial.h"

void print_elements_of_list(iSymbol *funcsym) {
    TList *templist = funcsym->data->args;
    int currlist_len = 0;

    //printf("%s\n", templist->first->name);
    while((templist->first != NULL) && (currlist_len<=funcsym->data->arg_count)) {
        printf("%d %s\n", templist->first->type,templist->first->name);
        templist->first = templist->first->next;
        currlist_len++;
    }

}

int bracket_counter = 0;
int params_counter = 0;

int nazov_len = 0;   //dlzka nazvu
char *nazov = NULL;  //samotny nazov
int symbol_type;
int funcname_len = 0;   //dlzka nazvu

clHTable* STable = NULL;      //STable = Classes_table
tHTable* ukazatel_na_triedu = NULL;  //ukazatel na triedu ............Class1 = ukazatel_na_triedu
locTable* local_table = NULL; //ukazatel na lokalnu tabulku u funkcie .............same
locSymbol* local_symbol = NULL;  //ukazatel na lokalny symbol
iSymbol* symbol = NULL;       //lubovolny symbol ............... funcsym = symbol
iSymbol* temp_symbol = NULL;  //docasny symbol
Hash_class* ptrclass = NULL;  //drzi ukazatel tHtable* ukazatel na triedu + meno triedy
char* classname;              //nazov classu
char* funcname;               //nazov aktualnej funckie

char* class_part = NULL;   //pri zlozenom ID tato cast obsahuje class
char* id_part = NULL;      //pri zlozenom ID tato cast obsahuje ID

bool priradenie = false;

int offset_counter = 0;    // counte na offset pri loc symboloch

int return_class()   //CLASS.ID rozdeli na class -> class_part a id -> id_part
{
   if(token2.data != NULL)
   {
      char *search = ".";
      nazov_len = strlen(token2.data);
      class_part = mymalloc(nazov_len*sizeof(char) + 2);
      id_part = mymalloc(nazov_len*sizeof(char) + 2);

      if(class_part == NULL || id_part == NULL)
         return INTERNAL_ERR;
      else
      {
         class_part = strtok(token2.data, search);
         id_part = strtok(NULL, search);

         if(class_part == NULL || id_part == NULL)
            return INTERNAL_ERR;
         return SUCCESS;
      }
   }
   else
      return INTERNAL_ERR;
}

int is_build_function() //ratam s tym ze token uz bol nacitany
{
   if(!(strcmp(token.data, "ifj16.print")))
      return F_print;
   else if(!(strcmp(token.data, "ifj16.readInt")))
      return F_int;
   else if(!(strcmp(token.data, "ifj16.readDouble")))
      return F_double;
   else if(!(strcmp(token.data, "ifj16.readString")))
      return F_string;
   else if(!(strcmp(token.data, "ifj16.length")))
      return F_length;
   else if(!(strcmp(token.data, "ifj16.substr")))
      return F_substr;
   else if(!(strcmp(token.data, "ifj16.compare")))
      return F_compare;
   else if(!(strcmp(token.data, "ifj16.find")))
      return F_find;
   else if(!(strcmp(token.data, "ifj16.sort")))
      return F_sort;
   else
      return F_NbIF;
}

int parser()
{
   STable = class_init();  //inicializacia tabulky symbolov
   if(STable == NULL)
      return INTERNAL_ERR;

   /************************************************************/
   /************************PRVY PRIECHOD***********************/
   /************************************************************/

   error = SUCCESS;
   get_token();
   if(error != SUCCESS)
      return error;

   if(token.stav == S_EOF)
      return SEMANTIC_PROG_ERR;

   error = prog();
   if(error != SUCCESS)
      return error;

   Hash_class* class_table_symbol = class_search(STable, "Main");

   if(class_table_symbol == NULL)   //overujeme, ci je definovana classa Main prave raz
   {
      fprintf(stderr, "SEMANTIC_PROG_ERR. Class Main must be defined exactly once. No more, no less.\n");
      return SEMANTIC_PROG_ERR;
   }
   if(Htab_search(class_table_symbol->ptr, "run") == NULL)
   {
      fprintf(stderr, "SEMANTIC_PROG_ERR. Class Main must contain one function called \"run\".\n");
      return SEMANTIC_PROG_ERR;
   }

   symbol = Htab_search(class_table_symbol->ptr, "run");
   if(symbol->data->args != NULL)
   {
      fprintf(stderr, "SEMANTIC_PROG_ERR. Function \"run\" is permitted to have arguments.\n");
      return SEMANTIC_PROG_ERR;
   }

   /************************************************************/
   /***********************DRUHY PRIECHOD***********************/
   /************************************************************/
   front_token();
   error = prog_scnd();
   if(error != SUCCESS)
      return error;

   return error;
}

//Pravidla:    P -> <C> <P>
//             P -> eof

int prog()
{
   if(error != SUCCESS)
      return error;

   if(!(strcmp(token.data, "class")))                   //pravidlo P -> C P
   {
      error = class();
      if(error != SUCCESS)
      {
         return error;
      }
   }
   else
   {
      return SYNTAX_ERR;
   }
   get_token();
   if(error != SUCCESS)
      return error;
   if(!(strcmp(token.data, "class"))) //nacital som class -> idem znova od zaciatku
   {
      error = prog();
      if(error != SUCCESS)
         return error;
   }
   if(token.stav != S_EOF)
      error = SYNTAX_ERR;

   if(error != SUCCESS)
      return error;
   return error;
}

//Pravidlo:    C -> class idc { <AC> }

int class()
{
   if(error != SUCCESS)
      return error;
   get_token();
   if(error != SUCCESS)
   return error;   //ocakavam id
   if(token.stav != S_ID) //Main musi byt prvy class
      return SYNTAX_ERR;

   if(!(strcmp(token.data, "ifj16")))  //class ifj16 nemoze byt definovany
   {
      fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"ifj16\" cannot be defined.\n");
      return SEMANTIC_PROG_ERR;
   }

   int classname_len = strlen(token.data);      //spracovanie nazvu triedy
   classname = mymalloc(classname_len*sizeof(char) + 2);
   if(classname == NULL)
   {
      error = INTERNAL_ERR;
      return error;
   }
   strcpy(classname,token.data);
   classname[strlen(token.data)+1] = '\0';   //az potialto

   get_token();
   if(error != SUCCESS)
   return error;   //ocakavam {
   if(token.stav != S_L_KOSZ)
      return SYNTAX_ERR;

   if(class_search(STable, classname) != NULL)
   {
      fprintf(stderr, "SEMANTIC_PROG_ERR. Class already defined.\n");
      return SEMANTIC_PROG_ERR;
   }

   ukazatel_na_triedu = HTab_init();   //inicializuje tabulku pre konkretnu triedu
   ptrclass = make_class(ukazatel_na_triedu, classname); //ukazatel na triedu + nazov triedy
   class_insert(STable, ptrclass);  //(ukazatel na celu tabulku, ukazatel na tabulku triedy)

   get_token();
   if(error != SUCCESS){
      return error;
    }
   error = after_class();
   if(error != SUCCESS)
      return error;

   if(token.stav != S_P_KOSZ)
      return SYNTAX_ERR;
   return error;

}

//Pravidla:    AC -> static <SD> <AC>
//             AC -> epsilon

int after_class()
{
   if(error != SUCCESS)
      return error;
   //ak pride static pravidlo: AC -> static <SD> <AC>
   if((strcmp(token.data, "static")))
      return SYNTAX_ERR;  
                //ak pride void pravidlo: <SD> -> void id <SDA>
   get_token();
   if(error != SUCCESS)
   return error; //inak pravidlo: <SD> -> <PARS> <DECL>;
   if(!(strcmp(token.data, "void")))
   {
      symbol_type = sym_type(token);  //vrati typ statickeho symbolu

      get_token();
      if(error != SUCCESS)
      return error;   //ocakavam id
      if(token.stav != S_ID)
         return SYNTAX_ERR;

      if(strchr(token.data, '.'))
      {
         fprintf(stderr, "Expected simple identiftier, \"%s\" is fully quialified ID.\n", token.data);
         return SYNTAX_ERR;
      }

      funcname_len = strlen(token.data); //vracia nazov statickeho symbolu
      funcname = mymalloc(funcname_len*sizeof(char) + 2);
      if(funcname == NULL)
      {
         error = INTERNAL_ERR;

         return error;
      }
      strcpy(funcname,token.data);
      funcname[strlen(token.data)+1] = '\0';

      if(Htab_search(ukazatel_na_triedu,token.data))
      {
         fprintf(stderr, "SEMANTIC_PROG_ERR. Function \"%s\" already defined in \"%s\"\n.", token.data, classname);
         return SEMANTIC_PROG_ERR;
      }

      symbol = sym_function_init(funcname, symbol_type, classname);
      Htab_insert(ukazatel_na_triedu, symbol);

      local_table = loc_table_init();  //inicializcia tabulky pre lokalne premenne vo funkcii
      sym_function_add_locals(symbol, local_table);

      //pouzite pravidlo <SDA> -> ( <PA> ) { <MB> }
      get_token();
      if(error != SUCCESS)
      return error; //musi byt lava zatvorka
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
      return error;   //ocakava bud ) v pripade prazndych argumentov, alebo type

      if(token.stav == S_PZAT)   //prazdny pocet argumentov
      {
         // pravidlo: <PA> -> epsilon

         get_token();
         if(error != SUCCESS)
         return error;
         if(token.stav != S_L_KOSZ)
            return SYNTAX_ERR;

         get_token();
         if(error != SUCCESS)
            return error;

         error = main_body();
         if(error != SUCCESS)
            return error;     ///TODO tu som doplnil za SYNTAX_ERR iba error

         //cakam }
         if(token.stav != S_P_KOSZ)
            return SYNTAX_ERR;

         get_token();
         if(error != SUCCESS)
            return error;
         if(!(strcmp(token.data, "static"))) //nachadza sa za telom funckie este nieco static?
            error = after_class();

         if(error != SUCCESS)
            return error;

         else if(token.stav == S_P_KOSZ)  //nactial som } znaciacu koniec classu
         {
            return error;
         }
         else
            return SYNTAX_ERR;
         return error;

      }
      else if(token.stav == S_KEY)  //funckia ma nejake parametre
      {
         if((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double")))
            return SYNTAX_ERR;

         params_counter = 0;
         error = params_after(); //riadenie predane params_after

         if(error != SUCCESS)
            return error;

         if(token.stav != S_PZAT)
            return SYNTAX_ERR;

         get_token();
         if(error != SUCCESS)
         return error;   //musi byt {
         if(token.stav != S_L_KOSZ)
            return SYNTAX_ERR;

         get_token();
         if(error != SUCCESS)
         return error;

         error = main_body();

         if(error != SUCCESS)
            return error;

         //cakam kosatu zavorku
         if(token.stav != S_P_KOSZ)
            return SYNTAX_ERR;

         get_token();
         if(error != SUCCESS)
            return error;
         if(!(strcmp(token.data, "static"))) //nachadza sa za telom funckie este nieco static?
            error = after_class();

         if(error != SUCCESS)
            return error;
         else if(token.stav == S_P_KOSZ)  //nactial som } znaciacu koniec classu
         {
            return error;
         }
         else
            return SYNTAX_ERR;
         return error;
      }
   }
   else if(!((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double"))))  //pravidlo <SD> -> <Pars> <Decl>
   {
      symbol_type = sym_type(token);  //vrati typ statickeho symbolu

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_ID)
         return SYNTAX_ERR;
      if(strchr(token.data, '.'))
      {
         fprintf(stderr, "Expected simple identiftier, \"%s\" is fully quialified ID.\n", token.data);
         return SYNTAX_ERR;
      }

      if(Htab_search(ukazatel_na_triedu,token.data))
      {
         fprintf(stderr, "SEMANTIC_PROG_ERR. Static variable \"%s\" already defined in \"%s\".\n", token.data, classname);
         return SEMANTIC_PROG_ERR;
      }

      nazov_len = strlen(token.data); //vracia nazov statickeho symbolu
      nazov = mymalloc(nazov_len*sizeof(char) + 2);
      if(nazov == NULL)
      {
         error = INTERNAL_ERR;

         return error;
      }
      strcpy(nazov,token.data);
      nazov[strlen(token.data)+1] = '\0';

      get_token();
      if(error != SUCCESS)
      return error;
      switch(token.stav)
      {
         case S_SEMICOLON:          //pravidlo <Decl> -> ;
                                    //(Tmeno, typ symbolu, inici, nazov cls, static, deklarovana)
            symbol = sym_variable_init(nazov, symbol_type, false, classname, true, true);
            Htab_insert(ukazatel_na_triedu, symbol);

            get_token();
            if(error != SUCCESS)
               return error;
            if(!(strcmp(token.data, "static")))
            {
               error = after_class();
               if(error != SUCCESS)
                  return error;
            }
            return error;
            break;

         case S_PRIR:               // pravidlo <Decl> ->

            symbol = sym_variable_init(nazov, symbol_type, true, classname, true, true);
            Htab_insert(ukazatel_na_triedu, symbol);

            while(token.stav != S_SEMICOLON)
            {
               get_token();
               if(error != SUCCESS)
                  return error;
               if(error != SUCCESS)
                  return error;
               if(token.stav == S_P_KOSZ || token.stav == S_EOF)
                  break;
            }

            if(token.stav != S_SEMICOLON)
               return SYNTAX_ERR;

            get_token();
            if(error != SUCCESS)
               return error;
            if(!(strcmp(token.data, "static")))
            {
               error = after_class();
               if(error != SUCCESS)
                  return error;
            }
            return error;
            break;

         case S_LZAT:               // pravidlo <Decl> -> ( <PA> ) { <MB> }

            symbol = sym_function_init(nazov, symbol_type, classname);
            Htab_insert(ukazatel_na_triedu, symbol);

            local_table = loc_table_init();  //inicializcia tabulky pre lokalne premenne vo funkcii
            sym_function_add_locals(symbol, local_table);

            funcname_len = strlen(nazov); //vracia nazov statickeho symbolu
            funcname = mymalloc(funcname_len*sizeof(char) + 2);
            if(funcname == NULL)
            {
               error = INTERNAL_ERR;

               return error;
            }
            strcpy(funcname,nazov);
            funcname[strlen(token.data)+1] = '\0';

            get_token();
            if(error != SUCCESS)
            return error;   //ocakava bud ) v pripade prazndych argumentov, alebo type

            if(token.stav == S_PZAT)   //prazdny pocet argumentov
            {
               //pravidlo: <PA> -> epsilon
               get_token();
               if(error != SUCCESS)
               return error;
               if(token.stav != S_L_KOSZ)
                  return SYNTAX_ERR;

               get_token();
               if(error != SUCCESS)
               return error;

               error = main_body();
               if(error != SUCCESS)
                  return error;

               //cakam }
               if(token.stav != S_P_KOSZ)
                  return SYNTAX_ERR;

               get_token();
               if(error != SUCCESS)
                  return error;
               if(!(strcmp(token.data, "static"))) //nachadza sa za telom funckie este nieco static?
                  error = after_class();

               if(error != SUCCESS)
                  return error;

               else if(token.stav == S_P_KOSZ)  //nactial som } znaciacu koniec classu
               {
                  return error;
               }
               else
                  return SYNTAX_ERR;
               return error;

            }
            else if(token.stav == S_KEY)
            {
               if((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double")))
                  return SYNTAX_ERR;

               params_counter = 0;
               error = params_after(); //riadenie predane params_after
               symbol->data->arg_count = params_counter;
               if(error != SUCCESS)
                  return error;

               if(token.stav != S_PZAT)
                  return SYNTAX_ERR;

               get_token();
               if(error != SUCCESS)
               return error;   //musi byt {
               if(token.stav != S_L_KOSZ)
                  return SYNTAX_ERR;

               get_token();
               if(error != SUCCESS)
               return error;

               error = main_body();
   
               if(error != SUCCESS)
                  return error;

               if(token.stav != S_P_KOSZ)
                  return SYNTAX_ERR;

               get_token();
               if(error != SUCCESS)
                  return error;
               if(!(strcmp(token.data, "static"))) //nachadza sa za telom funckie este nieco static?
                  error = after_class();

               if(error != SUCCESS)
                  return error;

               else if(token.stav == S_P_KOSZ)  //nactial som } znaciacu koniec classu
               {
                  return error;
               }
               else
                  return SYNTAX_ERR;
               return error;
            }

         default:
            return SYNTAX_ERR;
      }
   }

   return SYNTAX_ERR;
}

int params_after()
{
   params_counter++;
   symbol_type = sym_type(token);

   if(error != SUCCESS)
      return error;
   get_token();
   if(error != SUCCESS)
   return error;   //cakam id
   if(token.stav != S_ID)
      return SYNTAX_ERR;

   nazov_len = strlen(token.data); //vracia nazov  symbolu
   nazov = mymalloc(nazov_len*sizeof(char) + 2);
   if(nazov == NULL) 
   {
      error = INTERNAL_ERR;
      
      return error;
   }
   strcpy(nazov,token.data);
   nazov[strlen(token.data)+1] = '\0';

   local_symbol  = loc_symbol_init(nazov, symbol_type, true, true, funcname, 0);      //mozno bude treba namiesto classname, funcname
   loc_table_insert(local_table, local_symbol);

   function_add_args(symbol, nazov, symbol_type, params_counter);

   get_token();
   if(error != SUCCESS)
   return error;   //ak ) koncime a predavame riadenie, ak , pokracujeme
   if(token.stav == S_PZAT)
   {
      // pravidlo <NP> -> epsilon;
      return error;
   }
   else if(token.stav == S_CIARKA)
   {
      get_token();
      if(error != SUCCESS)
      return error; //cakam typ
      if((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double")))
            return SYNTAX_ERR;
      error = params_after();
      if(error != SUCCESS)
         return error;
   }
   else           // inak error
      return SYNTAX_ERR;
   return error; //sem by sme sa nemali nikdy dostat
}

int main_body()   //pravidlo <MB> -> <SL> <MB>
{
   if(error != SUCCESS)
      return error;

   if(!(strcmp(token.data, "while")))  //pravidlo <SL> -> while ( <E> ) { main body }
   {
      get_token();
      if(error != SUCCESS)
      return error;   //cakam (
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      bracket_counter = 0;

      while(bracket_counter != -1)  //preskakujeme vyrazy
      {
         get_token();
         if(error != SUCCESS)
            return error;
         if(token.stav == S_LZAT)
            bracket_counter++;
         else if(token.stav == S_PZAT)
            bracket_counter--;
         if(error != SUCCESS)
            return error;

         if(token.stav == S_P_KOSZ || token.stav == S_EOF)  //aby sme sa pri errore nezacykli
            break;
      }

      //get_token();
      if(error != SUCCESS)
      return error;   //cakam )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
      return error;   //za while je povinne {
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      error = main_body_riadiace();    // v tele whilu moze byt hocico, preto volame main_body

      if(error != SUCCESS)
            return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token.data, "if")))  //pravidlo <SL> -> if ( <E> ) { <MB> } else { <MB> }
   {
      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      bracket_counter = 0;

      while(bracket_counter != -1)  //preskakujeme vyrazy
      {
         get_token();
         if(error != SUCCESS)
            return error;
         if(token.stav == S_LZAT)
            bracket_counter++;
         else if(token.stav == S_PZAT)
            bracket_counter--;
         if(error != SUCCESS)
            return error;

         if(token.stav == S_P_KOSZ || token.stav == S_EOF)  //aby sme sa pri errore nezacykli
            break;
      }

      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      error = main_body_riadiace();
      if(error != SUCCESS)
         return error;

      if(error != SUCCESS)
         return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
         return error;

      if(strcmp(token.data, "else"))
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_P_KOSZ)
         error = main_body_riadiace();

      if(error != SUCCESS)
         return error;
      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(token.stav == S_ID)
   {
      int i = is_build_function();
      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav == S_PRIR)    //priradenie do premennej
      {
         while(token.stav != S_SEMICOLON)
         {
            get_token();
            if(error != SUCCESS)
               return error;
            if(error != SUCCESS)
               return error;
            if(token.stav == S_EOF || token.stav == S_P_KOSZ)
               break;
         }
      }
      else if(token.stav == S_LZAT)    //lubovalna ina funckia
      {
         if(i > 0)
         {
            error = build_function_call(i);
            if(error != SUCCESS)
               return error;
            if(token.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
         else
         {
            while(token.stav != S_PZAT)
            {
               get_token();
               if(error != SUCCESS)
                  return error;
               if(error != SUCCESS)
                  return error;
               if(token.stav == S_EOF || token.stav == S_P_KOSZ)
                  break;
            }
            if(token.stav != S_PZAT)
               return SYNTAX_ERR;

            get_token();
            if(error != SUCCESS)
               return error;
            if(token.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
      }
      else
         return SYNTAX_ERR;
   }
   else if(!((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double"))))  //pravidlo <SL> -> <PARS> <VD>
   {
      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_ID)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav == S_PRIR)
      {
         while(token.stav != S_SEMICOLON)
         {
            get_token();
            if(error != SUCCESS){
               return error;}
            if(error != SUCCESS)
               return error;
            if(token.stav == S_P_KOSZ || token.stav == S_EOF)
               break;
         }
         if(token.stav != S_SEMICOLON)
            return SYNTAX_ERR;
      }
      else if(token.stav == S_SEMICOLON)
         ;
      else
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token.data, "return")))
   {
         while(token.stav != S_SEMICOLON)
         {
            get_token();
            if(error != SUCCESS)
               return error;
            if(error != SUCCESS)
               return error;
            if(token.stav == S_P_KOSZ || token.stav == S_EOF)
               break;
         }
         if(token.stav != S_SEMICOLON)
            return SYNTAX_ERR;
   }
   else if(token.stav == S_L_KOSZ)
   {
      get_token();
      if(error != SUCCESS)
         return error;
      error = main_body();
      if(error != SUCCESS)
         return error;

      if(token.stav != S_P_KOSZ)
         return error;
   }
   else if(token.stav == S_P_KOSZ)
   {
      return error;
   }
   else if(token.stav == S_SEMICOLON || token.stav == S_LZAT || token.stav == S_PZAT || token.stav == S_PLUS
            || token.stav == S_INT || token.stav == S_DOUBLE || token.stav == S_MINUS || token.stav == S_MULTI
            || token.stav == S_DIV || token.stav == S_EOF || token.stav == S_PRIR || token.stav == S_MEN
            || token.stav == S_MENROV || token.stav == S_VAC || token.stav == S_VACROV || token.stav == S_ROVNY
            || token.stav == S_CIARKA || token.stav == S_VYKR || token.stav == S_NEROV
            || token.stav == S_STRING || token.stav == S_ESCAPE)
   {
      fprintf(stderr, "Syntax error, unexpeted token \"%s\".\n", token.data);
      return SYNTAX_ERR;
   }

   if(error != SUCCESS)
      return error;

   get_token();
   if(error != SUCCESS)
      return error;

   if(token.stav != S_P_KOSZ)
      error = main_body();

   if(error != SUCCESS)
      return error;

   if(token.stav == S_P_KOSZ)
      return error;
   return error;
}


int main_body_riadiace()   //pravidlo <MB> -> <SL> <MB>
{
   if(error != SUCCESS)
      return error;

   if(!(strcmp(token.data, "while")))  //pravidlo <MBr> -> while ( <E> ) { main body }
   {
      get_token();
      if(error != SUCCESS)
      return error;   //cakam (
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      bracket_counter = 0;

      while(bracket_counter != -1)  //preskakujeme vyrazy
      {
         get_token();
         if(error != SUCCESS)
            return error;
         if(token.stav == S_LZAT)
            bracket_counter++;
         else if(token.stav == S_PZAT)
            bracket_counter--;
         if(error != SUCCESS)
            return error;

         if(token.stav == S_P_KOSZ || token.stav == S_EOF)  //aby sme sa pri errore nezacykli
            break;
      }

      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
      return error;   //za while je povinne {
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      error = main_body_riadiace();    // v tele whilu moze byt hocico az na lokalnu deklaraciu, preto volame main_body

      if(error != SUCCESS)
            return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token.data, "if")))  //pravidlo <MBr> -> if ( <E> ) { <MB> } else { <MB> }
   {
      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      bracket_counter = 0;

      while(bracket_counter != -1)  //preskakujeme vyrazy
      {
         get_token();
         if(error != SUCCESS)
            return error;
         if(token.stav == S_LZAT)
            bracket_counter++;
         else if(token.stav == S_PZAT)
            bracket_counter--;

         if(error != SUCCESS)
            return error;
         if(token.stav == S_P_KOSZ || token.stav == S_EOF)  //aby sme sa pri errore nezacykli
            break;
      }

      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      error = main_body_riadiace();

      if(error != SUCCESS)
         return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
         return error;

      if(strcmp(token.data, "else"))
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_P_KOSZ)
         error = main_body_riadiace();

      if(error != SUCCESS)
         return error;
      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(token.stav == S_ID)
   {
      int i = is_build_function();
      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav == S_PRIR)    //priradenie do premennej
      {
         while(token.stav != S_SEMICOLON)
         {
            get_token();
            if(error != SUCCESS)
               return error;
            if(error != SUCCESS)
               return error;
            if(token.stav == S_P_KOSZ || token.stav == S_EOF)
               break;
         }
      }
      else if(token.stav == S_LZAT)    //lubovalna ina funckia
      {
         if(i > 0)
         {
            error = build_function_call(i);
            if(error != SUCCESS)
               return error;
         }
         else
         {
            while(token.stav != S_PZAT)
            {
               get_token();
               if(error != SUCCESS)
                  return error;
               if(error != SUCCESS)
                  return error;
               if(token.stav == S_EOF || token.stav == S_P_KOSZ)
                  break;
            }
            if(token.stav != S_PZAT)
               return SYNTAX_ERR;

            get_token();
            if(error != SUCCESS)
               return error;
            if(token.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
      }
      else
         return SYNTAX_ERR;
   }
   else if(!((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double"))))  //pravidlo <SL> -> <PARS> <VD>
   {
         return SEMANTIC_PROG_ERR;
   }
   else if(!(strcmp(token.data, "return")))
   {
         while(token.stav != S_SEMICOLON)
         {
            get_token();
            if(error != SUCCESS)
               return error;
            if(error != SUCCESS)
               return error;
            if(token.stav == S_P_KOSZ || token.stav == S_EOF)
               break;
         }
         if(token.stav != S_SEMICOLON)
            return SYNTAX_ERR;
   }
   else if(token.stav == S_L_KOSZ)
   {
      get_token();
      if(error != SUCCESS)
         return error;
      error = main_body_riadiace();

      if(error != SUCCESS)
         return error;

      if(token.stav != S_P_KOSZ)
         return error;
   }
   else if(token.stav == S_P_KOSZ)
   {
      return error;
   }
   else if(token.stav == S_SEMICOLON || token.stav == S_LZAT || token.stav == S_PZAT || token.stav == S_PLUS
            || token.stav == S_INT || token.stav == S_DOUBLE || token.stav == S_MINUS || token.stav == S_MULTI
            || token.stav == S_DIV || token.stav == S_EOF || token.stav == S_PRIR || token.stav == S_MEN
            || token.stav == S_MENROV || token.stav == S_VAC || token.stav == S_VACROV || token.stav == S_ROVNY
            || token.stav == S_CIARKA || token.stav == S_VYKR || token.stav == S_NEROV
            || token.stav == S_STRING || token.stav == S_ESCAPE)
   {
      fprintf(stderr, "Syntax error, unexpeted token \"%s\".\n", token.data);
      return SYNTAX_ERR;
   }

   if(error != SUCCESS)
      return error;

   get_token();
   if(error != SUCCESS)
      return error;
   //printf("%s\n", token.data);

   if(token.stav != S_P_KOSZ)
      error = main_body_riadiace();

   if(error != SUCCESS)
      return error;

   if(token.stav == S_P_KOSZ)
      return error;
   return error;
}


int build_function_call(int decider)
{
   if(error != SUCCESS)
      return error;
   if(decider == F_string || decider == F_double || decider == F_int)   //pravidlo <SL> -> <FC> -> read...();
   {
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
      return error;   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
      return error;   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;
   }
   else if(decider == F_length || decider == F_sort)
   {
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      switch(token.stav)
      {
         case S_STRING:    //pripad sort("awadawdawdaw");
            break;
         case S_ID:        //pripad sort(s);
            //overenie v TS ci je dany ID string
            break;
         default: return SEMANTIC_PROG_ERR;
      }

      get_token();
      if(error != SUCCESS)
      return error;   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
      return error;   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;
   }
   else if(decider == F_find || decider == F_compare)
   {
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      switch(token.stav)
      {
         case S_STRING:    //pripad find("awadawdawdaw", .....);
             break;
         case S_ID:        //pripad compare(s, .....);
            //overenie v TS ci je dany ID string
             break;
         default: return SEMANTIC_PROG_ERR;
      }

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      switch(token.stav)
      {
         case S_STRING:    //pripad find("awadawdawdaw", "awda");
             break;
         case S_ID:        //pripad compare(s, p);
            //overenie v TS ci je dany ID string
             break;
         default: return SEMANTIC_PROG_ERR;
      }

      get_token();
      if(error != SUCCESS)
      return error;   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
      return error;   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;
   }
   else if(decider == F_substr)
   {
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      switch(token.stav)
      {
         case S_STRING:    //pripad substr("awadawdawdaw", ...., .....);
             break;
         case S_ID:        //pripad substr(s, ...., .....);
            //overenie v TS ci je dany ID string
             break;
         default: return SEMANTIC_PROG_ERR;
      }

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      switch(token.stav)
      {
         case S_INT:    //pripad substr("awadawdawdaw", 2, .....);
             break;
         case S_ID:        //pripad substr("awadawdawdaw", smf, .....);
             break;
            //overenie v TS ci je dany ID string
         default: return SEMANTIC_PROG_ERR;
      }

      get_token();
      if(error != SUCCESS)
         return error;
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      get_token();
      if(error != SUCCESS)
         return error;
      switch(token.stav)
      {
         case S_INT:    //pripad substr("awadawdawdaw", 2, 9);
            break;
         case S_ID:        //pripad substr("awadawdawdaw", mi,lf);
             break;
            //overenie v TS ci je dany ID string
         default: return SEMANTIC_PROG_ERR;
      }

      get_token();
      if(error != SUCCESS)
      return error;   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(error != SUCCESS)
      return error;   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;
   }
   else if(decider == F_print)
   {
      error = build_print();
      if(error != SUCCESS)
         return error;
   }

   else
      return SYNTAX_ERR;
   return error;
}

int build_print()
{
   if(error != SUCCESS)
      return error;

   if(token.stav != S_LZAT)
      return SYNTAX_ERR;

   error = print_params();

   if(error != SUCCESS)
      return error;

   get_token();
   if(error != SUCCESS)
      return error;
   if(token.stav != S_SEMICOLON)
      return SYNTAX_ERR;

   return error;
}

int print_params()
{
   if(error != SUCCESS)
      return error;

   get_token();
   if(error != SUCCESS)
      return error;
   if(token.stav == S_ID)
      error = SUCCESS;
   else if(token.stav == S_STRING)
      error = SUCCESS;
   else return error;

   get_token();
   if(error != SUCCESS)
      return error;
   if(token.stav == S_PZAT)
   {
      return error;
   }
   else if(token.stav == S_PLUS)
   {
      error = print_params();
      if(error != SUCCESS)
         return error;
   }
   else
      return SYNTAX_ERR;
   return error;
}

/*-----------------------------------------------------------------------------------------------*/
/*---------------------------------------DRUHY PRIECHOD------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

symbolType assSymbol = tNan;

int is_build_function_scnd() //ratam s tym ze token uz bol nacitany
{
   if(!(strcmp(token2.data, "ifj16.print")))
      return F_print;
   else if(!(strcmp(token2.data, "ifj16.readInt")))
      return F_int;
   else if(!(strcmp(token2.data, "ifj16.readDouble")))
      return F_double;
   else if(!(strcmp(token2.data, "ifj16.readString")))
      return F_string;
   else if(!(strcmp(token2.data, "ifj16.length")))
      return F_length;
   else if(!(strcmp(token2.data, "ifj16.substr")))
      return F_substr;
   else if(!(strcmp(token2.data, "ifj16.compare")))
      return F_compare;
   else if(!(strcmp(token2.data, "ifj16.find")))
      return F_find;
   else if(!(strcmp(token2.data, "ifj16.sort")))
      return F_sort;
   else
      return F_NbIF;
}


int prog_scnd()
{
   if(error != SUCCESS)
      return error;

   if(!(strcmp(token2.data, "class")))                   //pravidlo P -> C P
   {
      error = class_scnd();
      if(error != SUCCESS)
      {
         return error;
      }
   }
   else
   {
      return SYNTAX_ERR;
   }
   front_token();
   if(!(strcmp(token2.data, "class"))) //nacital som class -> idem znova od zaciatku
   {
      error = prog_scnd();
   }
   if(token2.stav != S_EOF)
      error = SYNTAX_ERR;

   if(error != SUCCESS)
      return error;
   return error;
}

//Pravidlo:    C -> class idc { <AC> }

int class_scnd()
{
   if(error != SUCCESS)
      return error;
   front_token();   //ocakavam id
   if(token2.stav != S_ID)
      return SYNTAX_ERR;

   int classname_len = strlen(token2.data);      //Odtialto sa uchovava nazov
   classname = mymalloc(classname_len*sizeof(char) + 2);
   if(classname == NULL)
   {
      error = INTERNAL_ERR;
      return error;
   }
   strcpy(classname,token2.data);
   classname[strlen(token2.data)+1] = '\0';


   ptrclass = class_search(STable, classname);  //ptrclass->classname obsahuje nazov classu
                                                //ptrclass->ptr obsahuje ukazatel na tabulku danej classy
   if(ptrclass == NULL)
      return INTERNAL_ERR;

   front_token();   //ocakavam {
   if(token2.stav != S_L_KOSZ)
      return SYNTAX_ERR;

   front_token();
   error = after_class_scnd();
   if(error != SUCCESS)
      return error;

   if(token2.stav != S_P_KOSZ)
      return SYNTAX_ERR;
   return error;

}

//Pravidla:    AC -> static <SD> <AC>
//             AC -> epsilon

int after_class_scnd()
{
   if(error != SUCCESS)
      return error;
   //ak pride static pravidlo: AC -> static <SD> <AC>
   if((strcmp(token2.data, "static")))
      return SYNTAX_ERR;

                //ak pride void pravidlo: <SD> -> void id <SDA>
   front_token(); //inak pravidlo: <SD> -> <PARS> <DECL>;
   if(!(strcmp(token2.data, "void")))
   {
      front_token();   //ocakavam id
      if(token2.stav != S_ID)
         return SYNTAX_ERR;

      funcname_len = strlen(token2.data);      //Odtialto sa uchovava nazov
      funcname = mymalloc(funcname_len*sizeof(char) + 2);
      if(funcname == NULL)
      {
         error = INTERNAL_ERR;
         return error;
      }
      strcpy(funcname,token2.data);
      funcname[strlen(token2.data)+1] = '\0';

      symbol = Htab_search(ptrclass->ptr, funcname);  //symbol by mal teraz obsahovat nazov funkcie

      local_table = symbol->ptr_loctable;

      //pouzite pravidlo <SDA> -> ( <PA> ) { <MB> }
      front_token(); //musi byt lava zatvorka
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;

      front_token();   //ocakava bud ) v pripade prazndych argumentov, alebo type

      if(token2.stav == S_PZAT)   //prazdny pocet argumentov
      {
         // pravidlo: <PA> -> epsilon
         front_token();
         if(token2.stav != S_L_KOSZ)
            return SYNTAX_ERR;

         front_token();

         error = main_body_scnd();

         if(error != SUCCESS)
            return error;

         //cakam }
         if(token2.stav != S_P_KOSZ)
            return SYNTAX_ERR;

         front_token();
         if(!(strcmp(token2.data, "static"))) //nachadza sa za telom funckie este nieco static?
            error = after_class_scnd();

         if(error != SUCCESS)
            return error;

         else if(token2.stav == S_P_KOSZ)  //nactial som } znaciacu koniec classu
         {
            return error;
         }
         else
            return SYNTAX_ERR;
         return error;

      }
      else if(token2.stav == S_KEY)  //funckia ma nejake parametre
      {
         if((strcmp(token2.data, "String"))&&(strcmp(token2.data, "int"))&&(strcmp(token2.data, "double")))
            return SYNTAX_ERR;

         error = params_after_scnd(); //riadenie predane params_after

         if(error != SUCCESS)
            return error;

         if(token2.stav != S_PZAT)
            return SYNTAX_ERR;

         front_token();   //musi byt {
         if(token2.stav != S_L_KOSZ)
            return SYNTAX_ERR;

         front_token();

         error = main_body_scnd();

         if(error != SUCCESS)
            return error;

         //cakam kosatu zavorku
         if(token2.stav != S_P_KOSZ)
            return SYNTAX_ERR;

         front_token();
         if(!(strcmp(token2.data, "static"))) //nachadza sa za telom funckie este nieco static?
            error = after_class_scnd();

         if(error != SUCCESS)
            return error;
         else if(token2.stav == S_P_KOSZ)  //nactial som } znaciacu koniec classu
         {
            return error;
         }
         else
            return SYNTAX_ERR;
         return error;
      }
   }
   else if(!((strcmp(token2.data, "String"))&&(strcmp(token2.data, "int"))&&(strcmp(token2.data, "double"))))  //pravidlo <SD> -> <Pars> <Decl>
   {
      front_token();
      if(token2.stav != S_ID)
         return SYNTAX_ERR;

      temp_symbol = Htab_search(ptrclass->ptr, token2.data);
      if(temp_symbol != NULL)
      {
          assSymbol = temp_symbol->data->type;
      }

      nazov_len = strlen(token2.data);      //Odtialto sa uchovava nazov
      nazov = mymalloc(nazov_len*sizeof(char) + 2);
      if(nazov == NULL)
      {
         error = INTERNAL_ERR;
         return error;
      }
      strcpy(nazov,token2.data);
      nazov[strlen(token2.data)+1] = '\0';

      front_token();

      switch(token2.stav)
      {
         case S_SEMICOLON:          //pravidlo <Decl> -> ;
            front_token();
            if(!(strcmp(token2.data, "static")))
            {
               error = after_class_scnd();
               if(error != SUCCESS)
                  return error;
            }
            return error;
            break;

         case S_PRIR:               // pravidlo <Decl> ->

            front_token();
            error = expresion_parser();
            if(error != SUCCESS)
               return error;

            if(token2.stav != S_SEMICOLON)
               return SYNTAX_ERR;

            front_token();
            if(!(strcmp(token2.data, "static")))
            {
               error = after_class_scnd();
               if(error != SUCCESS)
                  return error;
            }
            return error;
            break;

         case S_LZAT:               // pravidlo <Decl> -> ( <PA> ) { <MB> }
            front_token();   //ocakava bud ) v pripade prazndych argumentov, alebo type

            funcname_len = strlen(nazov);      //Odtialto sa uchovava nazov
            funcname = mymalloc(funcname_len*sizeof(char) + 2);
            if(funcname == NULL)
            {
               error = INTERNAL_ERR;
               return error;
            }
            strcpy(funcname,nazov);
            funcname[strlen(nazov)+1] = '\0';

            symbol = Htab_search(ptrclass->ptr, funcname);
            local_table = symbol->ptr_loctable;

            if(token2.stav == S_PZAT)   //prazdny pocet argumentov
            {
               //pravidlo: <PA> -> epsilon
               front_token();
               if(token2.stav != S_L_KOSZ)
                  return SYNTAX_ERR;

               front_token();

               error = main_body_scnd();

               if(error != SUCCESS)
                  return error;

               //cakam }
               if(token2.stav != S_P_KOSZ)
                  return SYNTAX_ERR;

               front_token();
               if(!(strcmp(token2.data, "static"))) //nachadza sa za telom funckie este nieco static?
                  error = after_class_scnd();

               if(error != SUCCESS)
                  return error;

               else if(token2.stav == S_P_KOSZ)  //nactial som } znaciacu koniec classu
               {
                  return error;
               }
               else
                  return SYNTAX_ERR;
               return error;

            }
            else if(token2.stav == S_KEY)
            {
               if((strcmp(token2.data, "String"))&&(strcmp(token2.data, "int"))&&(strcmp(token2.data, "double")))
                  return SYNTAX_ERR;

               params_counter = 0;
               error = params_after_scnd(); //riadenie predane params_after
               if(error != SUCCESS)
                  return error;

               if(token2.stav != S_PZAT)
                  return SYNTAX_ERR;

               front_token();   //musi byt {
               if(token2.stav != S_L_KOSZ)
                  return SYNTAX_ERR;

               front_token();

               error = main_body_scnd();

               if(error != SUCCESS)
                  return error;

               if(token2.stav != S_P_KOSZ)
                  return SYNTAX_ERR;

               front_token();
               if(!(strcmp(token2.data, "static"))) //nachadza sa za telom funckie este nieco static?
                  error = after_class_scnd();

               if(error != SUCCESS)
                  return error;

               else if(token2.stav == S_P_KOSZ)  //nactial som } znaciacu koniec classu
               {
                  return error;
               }
               else
                  return SYNTAX_ERR;
               return error;
            }

         default:
            return SYNTAX_ERR;
      }
   }

   return error;
}

int params_after_scnd()
{
   /*params_counter++;
   symbol_type = sym_type(token2);*/

   if(error != SUCCESS)
      return error;
   front_token();   //cakam id
   if(token2.stav != S_ID)
      return SYNTAX_ERR;

   front_token();   //ak ) koncime a predavame riadenie, ak , pokracujeme
   if(token2.stav == S_PZAT)
   {
      // pravidlo <NP> -> epsilon;
      return error;
   }
   else if(token2.stav == S_CIARKA)
   {
      front_token(); //cakam typ
      if((strcmp(token2.data, "String"))&&(strcmp(token2.data, "int"))&&(strcmp(token2.data, "double")))
            return SYNTAX_ERR;
      error = params_after_scnd();
      if(error != SUCCESS)
         return error;
   }
   else           // inak error
      return SYNTAX_ERR;
   return error; //sem by sme sa nemali nikdy dostat
}

int main_body_scnd()   //pravidlo <MB> -> <SL> <MB>
{
   if(error != SUCCESS)
      return error;

   if(!(strcmp(token2.data, "while")))  //pravidlo <SL> -> while ( <E> ) { main body }
   {
      front_token();   //cakam (
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;

      front_token();
      error = expresion_parser();
      if(error != SUCCESS)
         return error;

      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;
      front_token();   //za while je povinne {
      if(token2.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      front_token();
      error = main_body_riadiace_scnd();    // v tele whilu moze byt hocico, preto volame main_body

      if(error != SUCCESS)
            return error;

      if(token2.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token2.data, "if")))  //pravidlo <SL> -> if ( <E> ) { <MB> } else { <MB> }
   {
      front_token();
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;

      front_token(); //vyhdnotenie vyrazu
      error = expresion_parser();
      if(error != SUCCESS)
         return error;

      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;

      front_token();
      if(token2.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      front_token();
      error = main_body_riadiace_scnd();
      if(error != SUCCESS)
         return error;

      if(error != SUCCESS)
         return error;

      if(token2.stav != S_P_KOSZ)
         return SYNTAX_ERR;
      front_token();

      if(strcmp(token2.data, "else"))
         return SYNTAX_ERR;

      front_token();
      if(token2.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      front_token();
      if(token2.stav != S_P_KOSZ)
         error = main_body_riadiace_scnd();

      if(error != SUCCESS)
         return error;
      if(token2.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(token2.stav == S_ID)
   {
      if(strchr(token2.data, '.'))  //identifikator je zlozeny
      {
         int i = is_build_function_scnd();
         if(i > 0)   //je to vstavana funkcia
         {
            front_token();
            error = build_function_call_scnd(i);
            if(error != SUCCESS)
               return error;
            if(token2.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
         else  //nie je to vstavana funkcia
         {
            error = return_class();
            if(error != SUCCESS)
               return error;

            Hash_class* class_table_symbol = class_search(STable, class_part);   //hladame ci existuje dana class

            if(class_table_symbol == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
               return SEMANTIC_PROG_ERR;
            }
            temp_symbol = Htab_search(class_table_symbol->ptr, id_part);
            if(temp_symbol == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
               return SEMANTIC_PROG_ERR;
            }
            if(temp_symbol->fce == false) //to co sme nasli je premmena
            {
               front_token();
               if(token2.stav != S_PRIR)  //za priradenim moze byt volanie funckie
                  return SYNTAX_ERR;

               assSymbol = temp_symbol->data->type;

               priradenie = true;
               temp_symbol->data->init = true;
               error = is_function_call_or_ass();
               if(error != SUCCESS)
                  return error;

               if(token2.stav != S_SEMICOLON)
                  return SYNTAX_ERR;
            }
            else if(temp_symbol->fce == true)   //to co sme nasli je funkcia
            {
               if(temp_symbol->data->type == tNan && priradenie == true)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. \"%s\" in \"%s\" has return type of void.\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
                  return RUNTIME_INIT_ERR;
               }
               error = user_function_call();
               if(error != SUCCESS)
                  return error;
            }
         }
      }
      else  //identifikator je jednoduchy
      {
         nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
         nazov = mymalloc(nazov_len*sizeof(char) + 2);
         if(nazov == NULL)
         {
            error = INTERNAL_ERR;

            return error;
         }
         strcpy(nazov,token2.data);
         nazov[nazov_len+1] = '\0';   //dostali sme nazov ID

         local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
         temp_symbol = Htab_search(ptrclass->ptr, nazov);  //overenei ci existuje ako funkcia

         if(local_symbol != NULL && temp_symbol == NULL)   //lokalny symbol sa nasiel -> je to premenna vo funkcii
         {
            front_token();
            if(token2.stav != S_PRIR)
               return SYNTAX_ERR;

            local_symbol->data->init = true;
            priradenie = true;
            assSymbol = local_symbol->data->type;
            error = is_function_call_or_ass();
            if(error != SUCCESS)
               return error;

            if(token2.stav != S_SEMICOLON)
               return SYNTAX_ERR;

         }
         else if(temp_symbol != NULL && local_symbol == NULL)  //nasiel sa globalny
         {
            if(temp_symbol->fce == true) //dany symbol sme nasli ako funkciu, z danej classy
            {
               error = user_function_call();
               if(error != SUCCESS)
                  return error;
            }
            else
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. When using \"%s\" you must enter fully quialified ID.\n", nazov);
               return SEMANTIC_PROG_ERR;
            }
         }
         else if(temp_symbol != NULL && local_symbol != NULL)   //pripad ze sa zhoduju staticka a lokalna ID tak sa bude pouzivat lokalna
         {
            front_token();
            if(token2.stav != S_PRIR)
               return SYNTAX_ERR;

            local_symbol->data->init = true;
            priradenie = true;
            assSymbol = local_symbol->data->type;
            error = is_function_call_or_ass();
            if(error != SUCCESS)
               return error;

            if(token2.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
         else  //nemalo by uz nastat
         {
            return SEMANTIC_PROG_ERR;
         }
      }
   }
   else if(!((strcmp(token2.data, "String"))&&(strcmp(token2.data, "int"))&&(strcmp(token2.data, "double"))))  //pravidlo <SL> -> <PARS> <VD>
   {
      symbol_type = sym_type(token2);

      front_token();
      if(token2.stav != S_ID)
         return SYNTAX_ERR;

      if(strchr(token2.data, '.'))  //nemoze byt plne kvalifikovane id
         return SYNTAX_ERR;

      nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
      nazov = mymalloc(nazov_len*sizeof(char) + 2);
      if(nazov == NULL)
      {
         error = INTERNAL_ERR;
         return error;
      }
      strcpy(nazov,token2.data);
      nazov[strlen(token2.data)+1] = '\0';

      local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje
      if(local_symbol != NULL)
      {
         fprintf(stderr, "Local variable \"%s\" alredy exists in \"%s\" function.\n", nazov, funcname);
         return SEMANTIC_PROG_ERR;
      }

      temp_symbol = Htab_search(ptrclass->ptr, nazov);  //overenei ci existuje ako funkcia
      if(temp_symbol != NULL)
      {
         if(temp_symbol->fce == true)
         {
         fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" which is declared as funcion in \"%s\".\n", nazov, classname);
         return SEMANTIC_PROG_ERR;
         }
      }

      front_token();
      if(token2.stav == S_PRIR)
      {
         local_symbol  = loc_symbol_init(nazov, symbol_type, true, true, classname, offset_counter++);      //mozno bude treba namiesto classname, funcname
         loc_table_insert(local_table, local_symbol);

         priradenie = true;
         assSymbol = local_symbol->data->type;
         error = is_function_call_or_ass();
         if(error != SUCCESS)
            return error;

         if(token2.stav != S_SEMICOLON)
            return SYNTAX_ERR;
      }
      else if(token2.stav == S_SEMICOLON)
      {
                                      //(Tmeno, typ symbolu, inci, decl, nazov_classu)
         local_symbol  = loc_symbol_init(nazov, symbol_type, false, true, classname, offset_counter++);
         loc_table_insert(local_table, local_symbol);
      }
      else
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token2.data, "return")))
   {
         front_token();
         error = expresion_parser();
         if(error != SUCCESS)
            return error;
         if(token2.stav != S_SEMICOLON)
            return SYNTAX_ERR;
   }
   else if(token2.stav == S_L_KOSZ)
   {
      front_token();
      error = main_body_scnd();

      if(error != SUCCESS)
         return error;

      if(token2.stav != S_P_KOSZ)
         return error;
   }
   else if(token2.stav == S_P_KOSZ)
   {
      return error;
   }
   else if(token2.stav == S_SEMICOLON || token2.stav == S_LZAT || token2.stav == S_PZAT || token2.stav == S_PLUS
            || token2.stav == S_INT || token2.stav == S_DOUBLE || token2.stav == S_MINUS || token2.stav == S_MULTI
            || token2.stav == S_DIV || token2.stav == S_EOF || token2.stav == S_PRIR || token2.stav == S_MEN
            || token2.stav == S_MENROV || token2.stav == S_VAC || token2.stav == S_VACROV || token2.stav == S_ROVNY
            || token2.stav == S_CIARKA || token2.stav == S_VYKR || token2.stav == S_NEROV
            || token2.stav == S_STRING || token2.stav == S_ESCAPE)
   {
      fprintf(stderr, "Syntax error, unexpeted token \"%s\".\n", token2.data);
      return SYNTAX_ERR;
   }

   if(error != SUCCESS)
      return error;

   front_token();

   if(token2.stav != S_P_KOSZ)
      error = main_body_scnd();

   if(error != SUCCESS)
      return error;

   if(token2.stav == S_P_KOSZ)
      return error;
   return error;
}


int main_body_riadiace_scnd()   //pravidlo <MB> -> <SL> <MB>
{
   if(error != SUCCESS)
      return error;

   if(!(strcmp(token2.data, "while")))  //pravidlo <MBr> -> while ( <E> ) { main body }
   {
      front_token();   //cakam (
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;

      front_token();
      error = expresion_parser();
      if(error != SUCCESS)
         return error;

      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;
      front_token();   //za while je povinne {
      if(token2.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      front_token();
      error = main_body_riadiace_scnd();    // v tele whilu moze byt hocico az na lokalnu deklaraciu, preto volame main_body

      if(error != SUCCESS)
            return error;

      if(token2.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token2.data, "if")))  //pravidlo <MBr> -> if ( <E> ) { <MB> } else { <MB> }
   {
      front_token();
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;

      front_token();
      error = expresion_parser();
      if(error != SUCCESS)
         return error;

      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;
      front_token();
      if(token2.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      front_token();
      error = main_body_riadiace_scnd();

      if(error != SUCCESS)
         return error;

      if(token2.stav != S_P_KOSZ)
         return SYNTAX_ERR;
      front_token();

      if(strcmp(token2.data, "else"))
         return SYNTAX_ERR;

      front_token();
      if(token2.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      front_token();
      if(token2.stav != S_P_KOSZ)
         error = main_body_riadiace_scnd();

      if(error != SUCCESS)
         return error;
      if(token2.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(token2.stav == S_ID)
   {
      if(strchr(token2.data, '.'))  //identifikator je zlozeny
      {
         int i = is_build_function_scnd();
         if(i > 0)   //je to vstavana funkcia
         {
            front_token();
            error = build_function_call_scnd(i);
            if(error != SUCCESS)
               return error;

            if(token2.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
         else  //nie je to vstavana funkcia
         {
            error = return_class();
            if(error != SUCCESS)
               return error;

            Hash_class* class_table_symbol = class_search(STable, class_part);   //hladame ci existuje dana class
            temp_symbol = Htab_search(class_table_symbol->ptr, id_part);

            if(class_table_symbol == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
               return SEMANTIC_PROG_ERR;
            }
            if(temp_symbol == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
               return SEMANTIC_PROG_ERR;
            }
            if(temp_symbol->fce == false) //to co sme nasli je premmena
            {
               front_token();
               if(token2.stav != S_PRIR)
                  return SYNTAX_ERR;

               priradenie = true;
               temp_symbol->data->init = true;
               assSymbol = temp_symbol->data->type;
               error = is_function_call_or_ass();
               if(error != SUCCESS)
                  return error;

               if(token2.stav != S_SEMICOLON)
                  return SYNTAX_ERR;
            }
            else if(temp_symbol->fce == true)   //to co sme nasli je funkcia
            {
               error = user_function_call();
               if(error != SUCCESS)
                  return error;
            }
         }
      }
      else  //identifikator je jednoduchy
      {
         nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
         nazov = mymalloc(nazov_len*sizeof(char) + 2);
         if(nazov == NULL)
         {
            error = INTERNAL_ERR;

            return error;
         }
         strcpy(nazov,token2.data);
         nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID

         local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
         temp_symbol = Htab_search(ptrclass->ptr, nazov);  //overenei ci existuje ako funkcia

         if(local_symbol != NULL && temp_symbol == NULL)   //lokalny symbol sa nasiel -> je to premenna vo funkcii
         {

            front_token();
            if(token2.stav != S_PRIR)
               return SYNTAX_ERR;

            priradenie = true;
            local_symbol->data->init = true;
            assSymbol = local_symbol->data->type;
            error = is_function_call_or_ass();
            if(error != SUCCESS)
               return error;

            if(token2.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
         else if(temp_symbol != NULL && local_symbol == NULL)
         {
            if(temp_symbol->fce == true) //dany symbol sme nasli ako funkciu
            {
               error = user_function_call();
               if(error != SUCCESS)
                  return error;
            }
         }
      }
   }
   else if(!((strcmp(token2.data, "String"))&&(strcmp(token2.data, "int"))&&(strcmp(token2.data, "double"))))  //pravidlo <SL> -> <PARS> <VD>
   {
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token2.data, "return")))
   {
         front_token();
         error = expresion_parser();
         if(error != SUCCESS)
            return error;
         if(token2.stav != S_SEMICOLON)
            return SYNTAX_ERR;
   }
   else if(token2.stav == S_L_KOSZ)
   {
      front_token();
      error = main_body_riadiace_scnd();

      if(error != SUCCESS)
         return error;

      if(token2.stav != S_P_KOSZ)
         return error;
   }
   else if(token2.stav == S_P_KOSZ)
   {
      return error;
   }
   else if(token2.stav == S_SEMICOLON || token2.stav == S_LZAT || token2.stav == S_PZAT || token2.stav == S_PLUS
            || token2.stav == S_INT || token2.stav == S_DOUBLE || token2.stav == S_MINUS || token2.stav == S_MULTI
            || token2.stav == S_DIV || token2.stav == S_EOF || token2.stav == S_PRIR || token2.stav == S_MEN
            || token2.stav == S_MENROV || token2.stav == S_VAC || token2.stav == S_VACROV || token2.stav == S_ROVNY
            || token2.stav == S_CIARKA || token2.stav == S_VYKR || token2.stav == S_NEROV
            || token2.stav == S_STRING || token2.stav == S_ESCAPE)
   {
      fprintf(stderr, "Syntax error, unexpeted token \"%s\".\n", token2.data);
      return SYNTAX_ERR;
   }

   if(error != SUCCESS)
      return error;

   front_token();

   if(token2.stav != S_P_KOSZ)
      error = main_body_riadiace_scnd();

   if(error != SUCCESS)
      return error;

   if(token2.stav == S_P_KOSZ)
      return error;
   return error;
}


int build_function_call_scnd(int decider)
{
   if(error != SUCCESS)
      return error;
   if(decider == F_string || decider == F_double || decider == F_int)   //pravidlo <SL> -> <FC> -> read...();
   {
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;
      front_token();   //musi byt )
      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;
      front_token();   //musi byt ;
      if(token2.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;
   }
   else if(decider == F_length || decider == F_sort)
   {
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;

      front_token();
      switch(token2.stav)
      {
         case S_STRING:    //pripad sort("awadawdawdaw");
            break;

         case S_ID:        //pripad sort(s);
            if(strchr(token2.data, '.'))
            {
               error = return_class();
               if(error != SUCCESS)
                  return error;
               Hash_class* class_table_symbol = class_search(STable, class_part);   //hladame ci existuje dana class

               if(class_table_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
                  return SEMANTIC_PROG_ERR;
               }
               temp_symbol = Htab_search(class_table_symbol->ptr, id_part);
               if(temp_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
                  return SEMANTIC_PROG_ERR;
               }
               if(temp_symbol->fce == true)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. \"%s\" defined as function, not variable in \"%s\".\n", id_part, class_part); //je to funckia, nie premenna
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->type != tString)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"String\" in \"%s\".\n", id_part);
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", id_part, funcname);
                  return RUNTIME_INIT_ERR;
               }

            }
            else
            {
               nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
               nazov = mymalloc(nazov_len*sizeof(char) + 2);
               if(nazov == NULL)
               {
                  error = INTERNAL_ERR;

                  return error;
               }
               strcpy(nazov,token2.data);
               nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID


               /*Hash_class* class_table_symbol = class_search(STable, classname);
               if(class_table_symbol != NULL)
               {
                  temp_symbol = Htab_search(class_table_symbol->ptr, nazov);
                  if(temp_symbol == NULL)
                  {
                     fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" which is not declared in funcion \"%s\".\n", nazov, funcname);
                     return SEMANTIC_PROG_ERR;
                  }
                  if(temp_symbol->fce == true && temp_symbol->isstatic == true)
                  {
                     fprintf(stderr, "SEMANTIC_TYPE_ERR. Using \"%s\" which is funcion in \"%s\".\n", nazov, funcname);
                     return SEMANTIC_TYPE_ERR;
                  }
               }
               else
                  return SEMANTIC_PROG_ERR; */

               local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
               if(local_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" undeclared in \"%s\".\n", nazov, funcname);
                  return SEMANTIC_PROG_ERR;
               }
               if(local_symbol->data->type != tString)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"String\" in \"%s\".\n", nazov);
                  return SEMANTIC_TYPE_ERR;
               }
               if(local_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", nazov, funcname);
                  return RUNTIME_INIT_ERR;
               }
            }
            break;

         default: return SEMANTIC_PROG_ERR;
      }

      front_token();   //musi byt )
      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;
      front_token();   //musi byt ;
      if(token2.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;
   }
   else if(decider == F_find || decider == F_compare)
   {
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;

      front_token();
      switch(token2.stav)
      {
         case S_STRING:    //pripad find("awadawdawdaw", .....);
             break;
         case S_ID:        //pripad compare(s, .....);
            if(strchr(token2.data, '.'))
            {
               error = return_class();
               if(error != SUCCESS)
                  return error;
               Hash_class* class_table_symbol = class_search(STable, class_part);   //hladame ci existuje dana class

               if(class_table_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
                  return SEMANTIC_PROG_ERR;
               }
               temp_symbol = Htab_search(class_table_symbol->ptr, id_part);
               if(temp_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
                  return SEMANTIC_PROG_ERR;
               }
               if(temp_symbol->fce == true)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. \"%s\" defined as function, not variable in \"%s\".\n", id_part, class_part); //je to funckia, nie premenna
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->type != tString)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"String\" in \"%s\".\n", id_part);
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", id_part, funcname);
                  return RUNTIME_INIT_ERR;
               }

            }
            else
            {
               nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
               nazov = mymalloc(nazov_len*sizeof(char) + 2);
               if(nazov == NULL)
               {
                  error = INTERNAL_ERR;

                  return error;
               }
               strcpy(nazov,token2.data);
               nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID

               /*Hash_class* class_table_symbol = class_search(STable, classname);
               if(class_table_symbol != NULL)
               {
                  temp_symbol = Htab_search(class_table_symbol->ptr, nazov);
                  if(temp_symbol == NULL)
                  {
                     fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" which is not declared in funcion \"%s\".\n", nazov, funcname);
                     return SEMANTIC_PROG_ERR;
                  }
                  if(temp_symbol->fce == true && temp_symbol->isstatic == true)
                  {
                     fprintf(stderr, "SEMANTIC_TYPE_ERR. Using \"%s\" which is funcion in \"%s\".\n", nazov, funcname);
                     return SEMANTIC_TYPE_ERR;
                  }
               }
               else
                  return SEMANTIC_PROG_ERR;  */

               local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
               if(local_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" undeclared in \"%s\".\n", nazov, funcname);
                  return SEMANTIC_PROG_ERR;
               }
               if(local_symbol->data->type != tString)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"String\" in \"%s\".\n", nazov);
                  return SEMANTIC_TYPE_ERR;
               }
               if(local_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", nazov, funcname);
                  return RUNTIME_INIT_ERR;
               }
            }
            break;
         default: return SEMANTIC_PROG_ERR;
      }

      front_token();
      if(token2.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      front_token();
      switch(token2.stav)
      {
         case S_STRING:    //pripad find("awadawdawdaw", "awda");
             break;
         case S_ID:        //pripad compare(s, p);
            if(strchr(token2.data, '.'))
            {
               error = return_class();
               if(error != SUCCESS)
                  return error;
               Hash_class* class_table_symbol = class_search(STable, class_part);   //hladame ci existuje dana class

               if(class_table_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
                  return SEMANTIC_PROG_ERR;
               }
               temp_symbol = Htab_search(class_table_symbol->ptr, id_part);
               if(temp_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
                  return SEMANTIC_PROG_ERR;
               }
               if(temp_symbol->fce == true)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. \"%s\" defined as function, not variable in \"%s\".\n", id_part, class_part); //je to funckia, nie premenna
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->type != tString)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"String\" in \"%s\".\n", id_part);
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", id_part, funcname);
                  return RUNTIME_INIT_ERR;
               }

            }
            else
            {
               nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
               nazov = mymalloc(nazov_len*sizeof(char) + 2);
               if(nazov == NULL)
               {
                  error = INTERNAL_ERR;

                  return error;
               }
               strcpy(nazov,token2.data);
               nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID

               /*Hash_class* class_table_symbol = class_search(STable, classname);
               if(class_table_symbol != NULL)
               {
                  temp_symbol = Htab_search(class_table_symbol->ptr, nazov);
                  if(temp_symbol == NULL)
                  {
                     fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" which is not declared in funcion \"%s\".\n", nazov, funcname);
                     return SEMANTIC_PROG_ERR;
                  }
                  if(temp_symbol->fce == true && temp_symbol->isstatic == true)
                  {
                     fprintf(stderr, "SEMANTIC_TYPE_ERR. Using \"%s\" which is funcion in \"%s\".\n", nazov, funcname);
                     return SEMANTIC_TYPE_ERR;
                  }
               }
               else
                  return SEMANTIC_PROG_ERR; */

               local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
               if(local_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" undeclared in \"%s\".\n", nazov, funcname);
                  return SEMANTIC_PROG_ERR;
               }
               if(local_symbol->data->type != tString)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"String\" in \"%s\".\n", nazov);
                  return SEMANTIC_TYPE_ERR;
               }
               if(local_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", nazov, funcname);
                  return RUNTIME_INIT_ERR;
               }
            }
            break;
         default: return SEMANTIC_PROG_ERR;
      }

      front_token();   //musi byt )
      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;
      front_token();   //musi byt ;
      if(token2.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;
   }
   else if(decider == F_substr)
   {
      if(token2.stav != S_LZAT)
         return SYNTAX_ERR;

      front_token();
      switch(token2.stav)
      {
         case S_STRING:    //pripad substr("awadawdawdaw", ...., .....);
             break;
         case S_ID:        //pripad substr(s, ...., .....);
            if(strchr(token2.data, '.'))
            {
               error = return_class();
               if(error != SUCCESS)
                  return error;
               Hash_class* class_table_symbol = class_search(STable, class_part);   //hladame ci existuje dana class

               if(class_table_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
                  return SEMANTIC_PROG_ERR;
               }
               temp_symbol = Htab_search(class_table_symbol->ptr, id_part);
               if(temp_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
                  return SEMANTIC_PROG_ERR;
               }
               if(temp_symbol->fce == true)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. \"%s\" defined as function, not variable in \"%s\".\n", id_part, class_part); //je to funckia, nie premenna
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->type != tString)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"String\" in \"%s\".\n", id_part);
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->init != false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", id_part, funcname);
                  return RUNTIME_INIT_ERR;
               }

            }
            else
            {
               nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
               nazov = mymalloc(nazov_len*sizeof(char) + 2);
               if(nazov == NULL)
               {
                  error = INTERNAL_ERR;

                  return error;
               }
               strcpy(nazov,token2.data);
               nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID

               /*Hash_class* class_table_symbol = class_search(STable, classname);
               if(class_table_symbol != NULL)
               {
                  temp_symbol = Htab_search(class_table_symbol->ptr, nazov);
                  if(temp_symbol == NULL)
                  {
                     fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" which is not declared in funcion \"%s\".\n", nazov, funcname);
                     return SEMANTIC_PROG_ERR;
                  }
                  if(temp_symbol->fce == true && temp_symbol->isstatic == true)
                  {
                     fprintf(stderr, "SEMANTIC_TYPE_ERR. Using \"%s\" which is funcion in \"%s\".\n", nazov, funcname);
                     return SEMANTIC_TYPE_ERR;
                  }
               }
               else
                  return SEMANTIC_PROG_ERR; */

               local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
               if(local_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" undeclared in \"%s\".\n", nazov, funcname);
                  return SEMANTIC_PROG_ERR;
               }
               if(local_symbol->data->type != tString)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"String\" in \"%s\".\n", nazov);
                  return SEMANTIC_TYPE_ERR;
               }
               if(local_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", nazov, funcname);
                  return RUNTIME_INIT_ERR;
               }
            }
            break;
         default: return SEMANTIC_PROG_ERR;
      }

      front_token();
      if(token2.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      front_token();
      switch(token2.stav)
      {
         case S_INT:    //pripad substr("awadawdawdaw", 2, .....);
             break;
         case S_ID:        //pripad substr("awadawdawdaw", smf, .....);
            if(strchr(token2.data, '.'))
            {
               error = return_class();
               if(error != SUCCESS)
                  return error;
               Hash_class* class_table_symbol = class_search(STable, class_part);   //hladame ci existuje dana class

               if(class_table_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
                  return SEMANTIC_PROG_ERR;
               }
               temp_symbol = Htab_search(class_table_symbol->ptr, id_part);
               if(temp_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
                  return SEMANTIC_PROG_ERR;
               }
               if(temp_symbol->fce == true)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. \"%s\" defined as function, not variable in \"%s\".\n", id_part, class_part); //je to funckia, nie premenna
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->type != tInt)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"int\" in \"%s\".\n", id_part);
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", id_part, funcname);
                  return RUNTIME_INIT_ERR;
               }

            }
            else
            {
               nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
               nazov = mymalloc(nazov_len*sizeof(char) + 2);
               if(nazov == NULL)
               {
                  error = INTERNAL_ERR;

                  return error;
               }
               strcpy(nazov,token2.data);
               nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID

               /*Hash_class* class_table_symbol = class_search(STable, classname);
               if(class_table_symbol != NULL)
               {
                  temp_symbol = Htab_search(class_table_symbol->ptr, nazov);
                  if(temp_symbol == NULL)
                  {
                     fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" which is not declared in funcion \"%s\".\n", nazov, funcname);
                     return SEMANTIC_PROG_ERR;
                  }
                  if(temp_symbol->fce == true && temp_symbol->isstatic == true)
                  {
                     fprintf(stderr, "SEMANTIC_TYPE_ERR. Using \"%s\" which is funcion in \"%s\".\n", nazov, funcname);
                     return SEMANTIC_TYPE_ERR;
                  }
               }
               else
                  return SEMANTIC_PROG_ERR;  */

               local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
               if(local_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" undeclared in \"%s\".\n", nazov, funcname);
                  return SEMANTIC_PROG_ERR;
               }
               if(local_symbol->data->type != tInt)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"int\" in \"%s\".\n", nazov);
                  return SEMANTIC_TYPE_ERR;
               }
               if(local_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", nazov, funcname);
                  return RUNTIME_INIT_ERR;
               }
            }
            break;
            //overenie v TS ci je dany ID string
         default: return SEMANTIC_PROG_ERR;
      }

      front_token();
      if(token2.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      front_token();
      switch(token2.stav)
      {
         case S_INT:    //pripad substr("awadawdawdaw", 2, 9);
            break;
         case S_ID:        //pripad substr("awadawdawdaw", mi,lf);
            if(strchr(token2.data, '.'))
            {
               error = return_class();
               if(error != SUCCESS)
                  return error;
               Hash_class* class_table_symbol = class_search(STable, class_part);   //hladame ci existuje dana class

               if(class_table_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
                  return SEMANTIC_PROG_ERR;
               }
               temp_symbol = Htab_search(class_table_symbol->ptr, id_part);
               if(temp_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
                  return SEMANTIC_PROG_ERR;
               }
               if(temp_symbol->fce == true)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. \"%s\" defined as function, not variable in \"%s\".\n", id_part, class_part); //je to funckia, nie premenna
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->type != tInt)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"int\" in \"%s\".\n", id_part);
                  return SEMANTIC_TYPE_ERR;
               }
               if(temp_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", id_part, funcname);
                  return RUNTIME_INIT_ERR;
               }

            }
            else
            {
               nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
               nazov = mymalloc(nazov_len*sizeof(char) + 2);
               if(nazov == NULL)
               {
                  error = INTERNAL_ERR;

                  return error;
               }
               strcpy(nazov,token2.data);
               nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID

               /*Hash_class* class_table_symbol = class_search(STable, classname);
               if(class_table_symbol != NULL)
               {
                  temp_symbol = Htab_search(class_table_symbol->ptr, nazov);
                  if(temp_symbol == NULL)
                  {
                     fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" which is not declared in funcion \"%s\".\n", nazov, funcname);
                     return SEMANTIC_PROG_ERR;
                  }
                  if(temp_symbol->fce == true && temp_symbol->isstatic == true)
                  {
                     fprintf(stderr, "SEMANTIC_TYPE_ERR. Using \"%s\" which is funcion in \"%s\".\n", nazov, funcname);
                     return SEMANTIC_TYPE_ERR;
                  }
               }
               else
                  return SEMANTIC_PROG_ERR; */

               local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
               if(local_symbol == NULL)
               {
                  fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" undeclared in \"%s\".\n", nazov, funcname);
                  return SEMANTIC_PROG_ERR;
               }
               if(local_symbol->data->type != tInt)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Expected type \"int\" in \"%s\".\n", nazov);
                  return SEMANTIC_TYPE_ERR;
               }
               if(local_symbol->data->init == false)
               {
                  fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", nazov, funcname);
                  return RUNTIME_INIT_ERR;
               }
            }
            break;
            //overenie v TS ci je dany ID string
         default: return SEMANTIC_PROG_ERR;
      }

      front_token();   //musi byt )
      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;
      front_token();   //musi byt ;
      if(token2.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;
   }
   else if(decider == F_print)
   {
      error = build_print_scnd();
      if(error != SUCCESS)
         return error;
   }

   else
      return SYNTAX_ERR;
   return error;
}

int build_print_scnd()
{
   if(error != SUCCESS)
      return error;

   if(token2.stav != S_LZAT)
      return SYNTAX_ERR;

   error = print_params_scnd();

   if(error != SUCCESS)
      return error;

   front_token();
   if(token2.stav != S_SEMICOLON)
      return SYNTAX_ERR;

   return error;
}

int print_params_scnd()
{
   if(error != SUCCESS)
      return error;

   front_token();
   if(token2.stav == S_ID)
      error = SUCCESS;
   else if(token2.stav == S_STRING)
      error = SUCCESS;
   else return error;

   front_token();
   if(token2.stav == S_PZAT)
   {
      return error;
   }
   else if(token2.stav == S_PLUS)
   {
      error = print_params_scnd();
      if(error != SUCCESS)
         return error;
   }
   else
      return SYNTAX_ERR;
   return error;
}

int user_function_call()
{
   front_token();
   if(token2.stav != S_LZAT)
   {
      fprintf(stderr, "SYNTAX_ERR. Missing \"(\" when calling function \"%s\" in funcion \"%s\".\n", id_part, funcname);
      return SYNTAX_ERR;
   }


   if(temp_symbol->data->arg_count == 0) //temp_symbol = ukazatel na funkciu, ktorej ideme overit parametre
   {
      front_token();
      if(token2.stav == S_ID || token2.stav == S_STRING || token2.stav == S_INT || token2.stav == S_DOUBLE)
      {
         fprintf(stderr, "SEMANTIC_TYPE_ERR. Function \"%s.%s\" doesnt have any parameters.\n", class_part, id_part);
         return SEMANTIC_TYPE_ERR;
      }
      if(token2.stav != S_PZAT)
         return SYNTAX_ERR;
     
     front_token();
      if(token2.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;

   }
   else if(temp_symbol->data->arg_count != 0)
   {
      int i = temp_symbol->data->arg_count;
      TNode *Node = temp_symbol->data->args->first;   //ukazatel na jeden argument
      symbolType argument_type = Node->type;  //v argument type mame typ prveho argumentu funkcie
      while(i > 0)
      {
         front_token();
         if(token2.stav == S_CIARKA)
         {
            continue;
         }
         else if(token2.stav == S_PZAT)
         {
            return SEMANTIC_TYPE_ERR;
         }
         else if(strchr(token2.data, '.'))  //zlozeny id
         {
            error = return_class();
            if(error != SUCCESS)
               return error;

            Hash_class* argument_class = class_search(STable, class_part);   //hladame ci existuje dana class

            if(argument_class == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
               return SEMANTIC_PROG_ERR;
            }

            iSymbol* id_symbol = Htab_search(argument_class->ptr, id_part); //v danej classe hladame symbol
            if(id_symbol == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
               return SEMANTIC_PROG_ERR;
            }
            if(id_symbol->fce == true) //to co sme nasli je premmena
            {
               fprintf(stderr, "SEMANTIC_TYPE_ERR. \"%s\" is function, not variable in class \"%s\".\n", id_part, class_part);
               return SEMANTIC_TYPE_ERR;
            }
            if(id_symbol->data->init == false)
            {
               fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", id_part, funcname);
               return RUNTIME_INIT_ERR;
            }
            else if(id_symbol->fce == false) //je to premenna
            {
               if(argument_type != id_symbol->data->type)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Wrong argument type of \"%s\" in function \"%s\".\n", id_part, class_part);
                  return SEMANTIC_TYPE_ERR;
               }
               if(i - 1 > 0)
               {
                  Node = Node->next;
                  argument_type = Node->type;
               }
            }
         }
         else  //ID je jednoduche   /****************************************************************************************************************/
         {
            nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
            nazov = mymalloc(nazov_len*sizeof(char) + 2);
            if(nazov == NULL)
            {
               error = INTERNAL_ERR;

               return error;
            }
            strcpy(nazov,token2.data);
            nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID

            local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna existuje ako premenna
            if(local_symbol == NULL)

            /*temp_symbol = Htab_search(ptrclass->ptr, nazov);  //overenei ci existuje ako funkcia
            if(temp_symbol == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" which is not declared in funcion \"%s\".\n", nazov, funcname);
               return SEMANTIC_TYPE_ERR;
            }
            if(temp_symbol->fce == true && temp_symbol->isstatic == true)
            {
               fprintf(stderr, "SEMANTIC_TYPE_ERR. Using \"%s\" which is funcion in \"%s\".\n", nazov, funcname);
               return SEMANTIC_PROG_ERR;
            }*/
            if(local_symbol == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. Using \"%s\" undeclared in \"%s\".\n", nazov, funcname);
               return SEMANTIC_PROG_ERR;
            }
            if(local_symbol->data->init == false)
            {
               fprintf(stderr, "RUNTIME_INIT_ERR. Using uninitialized \"%s\" in \"%s\".\n", nazov, funcname);
               return RUNTIME_INIT_ERR;
            }
            else if(local_symbol->data->init == true)
            {
               if(argument_type != local_symbol->data->type)
               {
                  fprintf(stderr, "SEMANTIC_TYPE_ERR. Wrong argument type of \"%s\" in function \"%s\".\n", nazov, funcname);
                  return SEMANTIC_TYPE_ERR;
               }
               if(i - 1 > 0)
               {
                  Node = Node->next;
                  argument_type = Node->type;
               }
            }
         }

         i--;
      }
   }
   front_token();
   if(token2.stav != S_PZAT)
      return SYNTAX_ERR;
   front_token();
   if(token2.stav != S_SEMICOLON)
      return SYNTAX_ERR;
   return error;
}

int is_function_call_or_ass()
{

front_token();
if(token2.stav != S_ID)
{
   error = expresion_parser();
   if(error != SUCCESS)
      return error;
}
else
{
   if(strchr(token2.data, '.'))  //zlozeny ID
   {
      int i = is_build_function_scnd();
      if(i > 0)   //vstavana funkcia
      {
         if(i == 1 && priradenie == true)
         {
            fprintf(stderr, "RUNTIME_INIT_ERR. \"ifj16.print\" has return type of void.\n"); //dana staticka premenna/funckia neexistuje
            return RUNTIME_INIT_ERR;
         }
         front_token();
         error = build_function_call_scnd(i);
         if(error != SUCCESS)
            return error;
         if(token2.stav != S_SEMICOLON)
            return error;
      }
      else
      {
         error = return_class();
         if(error != SUCCESS)
            return error;
         Hash_class* isfunc_symbol = class_search(STable, class_part);  //hladane danu classu, isfunc - ukazatel na classu
         if(isfunc_symbol == NULL)
         {
            fprintf(stderr, "SEMANTIC_PROG_ERR. Class \"%s\" undefined.\n", class_part);  //dana classa neexsituje
            return SEMANTIC_PROG_ERR;
         }
         else
         {
            temp_symbol = Htab_search(isfunc_symbol->ptr, id_part);
            if(temp_symbol == NULL)
            {
               fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in class \"%s\".\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
               return SEMANTIC_PROG_ERR;
            }
            else  //nie vstavana funkcia
            {
               if(temp_symbol->fce == false) //nie je to funkcia, je to globlana premenna
               {
                  error = expresion_parser();
                  if(error != SUCCESS)
                     return error;
               }
               else  //je to funkcia
               {
                  if(temp_symbol->data->type == tNan && priradenie == true)
                  {
                     fprintf(stderr, "RUNTIME_INIT_ERR. \"%s\" in \"%s\" has return type of void.\n", id_part, class_part); //dana staticka premenna/funckia neexistuje
                     return RUNTIME_INIT_ERR;
                  }
                  /*printf("%d\n", temp_symbol->data->type);
                  printf("%d\n", assSymbol);*/
                  if(temp_symbol->data->type != assSymbol && priradenie == true)
                  {
                     fprintf(stderr, "RUNTIME_INIT_ERR. Colliding return type of function \"%s\"\n", id_part); //dana staticka premenna/funckia neexistuje
                     return SEMANTIC_TYPE_ERR;
                  }
                  error = user_function_call();
                  if(error != SUCCESS)
                     return error;
               }
            }
         }
      }
   }
   else  //jednoduchy ID
   {
      nazov_len = strlen(token2.data); //vracia nazov statickeho symbolu
      nazov = mymalloc(nazov_len*sizeof(char) + 2);
      if(nazov == NULL)
      {
         error = INTERNAL_ERR;

         return error;
      }
      strcpy(nazov,token2.data);
      nazov[strlen(token2.data)+1] = '\0';   //dostali sme nazov ID

      local_symbol = loc_symbol_search(local_table, nazov); //overenie, ci uz dana lokalna premenna neexistuje ako premenna
      temp_symbol = Htab_search(ptrclass->ptr, nazov);  //overenei ci existuje ako funkcia
      if(local_symbol != NULL)   //existuje ako lokalna premenna
      {
         error = expresion_parser();
         if(error != SUCCESS)
            return error;
      }
      else if(local_symbol == NULL && temp_symbol != NULL)
      {
         if(temp_symbol->fce == true)
         {
            if(temp_symbol->data->type == tNan && priradenie == true)
            {
               fprintf(stderr, "RUNTIME_INIT_ERR. \"%s\" in \"%s\" has return type of void.\n", nazov, class_part); //dana staticka premenna/funckia neexistuje
               return RUNTIME_INIT_ERR;
            }
            if(temp_symbol->data->type != assSymbol && priradenie == true)
            {
               fprintf(stderr, "RUNTIME_INIT_ERR. Colliding return type of function \"%s\"\n", id_part); //dana staticka premenna/funckia neexistuje
               return SEMANTIC_TYPE_ERR;
            }
            error = user_function_call();
            if(error != SUCCESS)
               return error;
         }
      }
      else
      {
         fprintf(stderr, "SEMANTIC_PROG_ERR. \"%s\" undefined in function \"%s\".\n", nazov, funcname); //dana staticka premenna/funckia neexistuje
            return SEMANTIC_PROG_ERR;
      }
   }
   priradenie = false;
   return error;
   }
return error;
}
