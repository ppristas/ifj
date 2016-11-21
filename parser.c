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
#include "stack.h"
#include "parser.h"
//#include "scaner.h"
#include "error.h"
#include "preced.h"

/*bool BI_readInt = false;
bool BI_readDouble = false;
bool BI_readString = false;
bool BI_length = false;
bool BI_substr = false;
bool BI_compare = false;
bool BI_find = false;
bool BI_sort = false;*/

tStack p_stack;

/*
void expand(tStack *p_stack,int num,...)   //rozsiri neterminal na neterminaly-terminaly
{  
   stackPop(p_stack);  //Pop neterminalu na vrchu
   va_list valist;
   va_start(valist, num);
   for(int i = 0; i < num; i++)
   {
      stackPush(p_stack, va_arg(valist, int));  //napushovanie terminalov-neterminalov
   }
   return;
}
*/

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
   error = SUCCESS;
   get_token();

   error = prog();   //prvy priechod   
   if(error != SUCCESS)
      return error;

   front_token();
   error = prog_scnd();    //druhy priechod
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
   if(!(strcmp(token.data, "class"))) //nacital som class -> idem znova od zaciatku
   {
      error = prog();
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
   get_token();   //ocakavam id
   if(token.stav != S_ID) //Main musi byt prvy class
      return SYNTAX_ERR;
   if(!(strcmp(token.data, "ifj16")))  //class ifj16 nemoze byt definovany
      return SEMANTIC_PROG_ERR;

   get_token();   //ocakavam {
   if(token.stav != S_L_KOSZ)
      return SYNTAX_ERR;

   get_token();
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
   get_token(); //inak pravidlo: <SD> -> <PARS> <DECL>;
   if(!(strcmp(token.data, "void")))
   {
      
      get_token();   //ocakavam id
      if(token.stav != S_ID)
         return SYNTAX_ERR;
      
      //pouzite pravidlo <SDA> -> ( <PA> ) { <MB> }
      get_token(); //musi byt lava zatvorka
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      
      get_token();   //ocakava bud ) v pripade prazndych argumentov, alebo type
   
      if(token.stav == S_PZAT)   //prazdny pocet argumentov
      {
         // pravidlo: <PA> -> epsilon
         get_token();   
         if(token.stav != S_L_KOSZ)
            return SYNTAX_ERR;
         
         get_token();
         error = main_body();

         if(error != SUCCESS)
            return SYNTAX_ERR;

         //cakam }
         if(token.stav != S_P_KOSZ)
            return SYNTAX_ERR;

         get_token();
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

         error = params_after(); //riadenie predane params_after  

         if(error != SUCCESS)
            return error;

         if(token.stav != S_PZAT)
            return SYNTAX_ERR;

         get_token();   //musi byt {
         if(token.stav != S_L_KOSZ)
            return SYNTAX_ERR;
         
         get_token(); 
         error = main_body();

         if(error != SUCCESS)
            return error;

         //cakam kosatu zavorku
         if(token.stav != S_P_KOSZ)
            return SYNTAX_ERR;

         get_token();
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
      get_token();
      if(token.stav != S_ID)
         return SYNTAX_ERR; 
      get_token();          

      switch(token.stav)
      {
         case S_SEMICOLON:          //pravidlo <Decl> -> ;
            get_token();
            if(!(strcmp(token.data, "static")))
            {
               error = after_class();
               if(error != SUCCESS)
                  return error;
            }
            return error;
            break;

         case S_PRIR:               // pravidlo <Decl> ->
            while(token.stav != S_SEMICOLON)
            {
               get_token();
               if(error != SUCCESS)
                  return error;
            }

            if(token.stav != S_SEMICOLON)
               return SYNTAX_ERR;

            get_token();
            if(!(strcmp(token.data, "static")))
            {
               error = after_class();
               if(error != SUCCESS)
                  return error;
            }
            return error;
            break;

         case S_LZAT:               // pravidlo <Decl> -> ( <PA> ) { <MB> }
            get_token();   //ocakava bud ) v pripade prazndych argumentov, alebo type
                     
            if(token.stav == S_PZAT)   //prazdny pocet argumentov
            {
               //pravidlo: <PA> -> epsilon
               get_token();   
               if(token.stav != S_L_KOSZ)
                  return SYNTAX_ERR;
                 
               get_token();             
               error = main_body();

               if(error != SUCCESS)
                  return error;

               //cakam }
               if(token.stav != S_P_KOSZ)
                  return SYNTAX_ERR;   

               get_token();
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

               error = params_after(); //riadenie predane params_after                   
               if(error != SUCCESS)
                  return error;

               if(token.stav != S_PZAT)
                  return SYNTAX_ERR;

               get_token();   //musi byt {
               if(token.stav != S_L_KOSZ)
                  return SYNTAX_ERR;
                
               get_token();  
               error = main_body();

               if(error != SUCCESS)
                  return error;

               if(token.stav != S_P_KOSZ)
                  return SYNTAX_ERR;
               
               get_token();
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

   return error;
}

int params_after()
{
   if(error != SUCCESS)
      return error;
   get_token();   //cakam id
   if(token.stav != S_ID)
      return SYNTAX_ERR;
   get_token();   //ak ) koncime a predavame riadenie, ak , pokracujeme
   if(token.stav == S_PZAT)
   {
      // pravidlo <NP> -> epsilon;
      return error;
   }
   else if(token.stav == S_CIARKA)
   {
      get_token(); //cakam typ
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
      get_token();   //cakam (
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      /********************************VYRAZ*********************************/

      get_token();   //cakam )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //za while je povinne {
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      error = main_body_riadiace();    // v tele whilu moze byt hocico, preto volame main_body
      
      if(error != SUCCESS)
            return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token.data, "if")))  //pravidlo <SL> -> if ( <E> ) { <MB> } else { <MB> }
   {
      get_token();
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      /****************VYHODNOTENIE VYRAZU*******************/

      get_token();
      if(token.stav != S_INT)
         return FILIP_ERR;


      /*************************ZATIAL INT FAKE**************/
      get_token();
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      error = main_body_riadiace();
      if(error != SUCCESS)
         return error;
     
      if(error != SUCCESS)
         return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
      get_token();

      if(strcmp(token.data, "else")) 
         return SYNTAX_ERR;      

      get_token();
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
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
      if(token.stav == S_PRIR)    //priradenie do premennej
      {  
         //provizorne
         get_token();
         if(token.stav != S_SEMICOLON)
            return SYNTAX_ERR;
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
            /***********VYRAZ**************/
            get_token();
            if(token.stav != S_PZAT)
               return SYNTAX_ERR;

            get_token();
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
      if(token.stav != S_ID)
         return SYNTAX_ERR;

      get_token();
      if(token.stav == S_PRIR)
      {
         /**********************ZAVOLAT OVERENIE VYRAZU***************************/
         get_token();
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
         //zatial takto inak tu bude vyraz      
         get_token();
         if(token.stav != S_SEMICOLON)
            return SYNTAX_ERR;
   }
   else if(token.stav == S_L_KOSZ)
   {
      get_token();
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
      fprintf(stderr, "Syntax error, unexpeted token \"%s\"\n.", token.data);
      return SYNTAX_ERR;
   }   

   if(error != SUCCESS)
      return error;

   get_token();
   //printf("%s\n", token.data); 
  
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
      get_token();   //cakam (
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      /********************************VYRAZ*********************************/

      get_token();   //cakam )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //za while je povinne {
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      error = main_body_riadiace();    // v tele whilu moze byt hocico az na lokalnu deklaraciu, preto volame main_body
      
      if(error != SUCCESS)
            return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token.data, "if")))  //pravidlo <MBr> -> if ( <E> ) { <MB> } else { <MB> }
   {
      get_token();
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      /****************VYHODNOTENIE VYRAZU*******************/

      get_token();
      if(token.stav != S_INT)
         return FILIP_ERR;

      /*************************ZATIAL INT FAKE**************/
      get_token();
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      error = main_body_riadiace();
     
      if(error != SUCCESS)
         return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
      get_token();

      if(strcmp(token.data, "else")) 
         return SYNTAX_ERR;      

      get_token();
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
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
      if(token.stav == S_PRIR)    //priradenie do premennej
      {  
         //provizorne
         get_token();
         if(token.stav != S_SEMICOLON)
            return SYNTAX_ERR;
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
            /***********VYRAZ**************/
            get_token();
            if(token.stav != S_PZAT)
               return SYNTAX_ERR;

            get_token();
            if(token.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
      }
      else
         return SYNTAX_ERR;
   }
   else if(!((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double"))))  //pravidlo <SL> -> <PARS> <VD>
   {  
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token.data, "return")))
   {
         //zatial takto inak tu bude vyraz      
         get_token();
         if(token.stav != S_SEMICOLON)
            return SYNTAX_ERR;
   }
   else if(token.stav == S_L_KOSZ)
   {
      get_token();
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
      fprintf(stderr, "Syntax error, unexpeted token \"%s\"\n.", token.data);
      return SYNTAX_ERR;
   }

   if(error != SUCCESS)
      return error;

   get_token();
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
      get_token();   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;  
   }
   else if(decider == F_length || decider == F_sort)
   {
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      
      get_token();
      switch(token.stav)
      {
         case S_STRING:    //pripad sort("awadawdawdaw");
             break;
         case S_ID:        //pripad sort(s);
            //overenie v TS ci je dany ID string
             break;
         default: return SEMANTIC_PROG_ERR;
      }  

         return SYNTAX_ERR;
      get_token();   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR; 
      return error;                       
   }
   else if(decider == F_find || decider == F_compare)
   {
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      
      get_token();
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
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      get_token();
      switch(token.stav)
      {
         case S_STRING:    //pripad find("awadawdawdaw", "awda");
             break;
         case S_ID:        //pripad compare(s, p);
            //overenie v TS ci je dany ID string
             break;
         default: return SEMANTIC_PROG_ERR;
      } 

      get_token();   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;    
   }
   else if(decider == F_substr)
   {
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      get_token();
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
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      get_token();
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
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      get_token();
      switch(token.stav)
      {
         case S_INT:    //pripad substr("awadawdawdaw", 2, 9);
            break;
         case S_ID:        //pripad substr("awadawdawdaw", mi,lf);
             break;
            //overenie v TS ci je dany ID string
         default: return SEMANTIC_PROG_ERR;
      } 

      get_token();   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //musi byt ;
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
   if(token.stav != S_SEMICOLON)
      return SYNTAX_ERR;

   return error;
}

int print_params()
{
   if(error != SUCCESS)
      return error;

   get_token();
   if(token.stav == S_ID)
      error = SUCCESS;
   else if(token.stav == S_STRING)
      error = SUCCESS;
   else return error;

   get_token();
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
   if(token2.stav != S_ID) //Main musi byt prvy class
      return SYNTAX_ERR;
   if(!(strcmp(token2.data, "ifj16")))  //class ifj16 nemoze byt definovany
      return SEMANTIC_PROG_ERR;

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
            return SYNTAX_ERR;

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
      /********************************VYRAZ*********************************/

      front_token();   //cakam )
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
      /****************VYHODNOTENIE VYRAZU*******************/

      front_token();
      if(token2.stav != S_INT)
         return FILIP_ERR;


      /*************************ZATIAL INT FAKE**************/
      front_token();
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
      int i = is_build_function_scnd();
      front_token();
      if(token2.stav == S_PRIR)    //priradenie do premennej
      {  
         //provizorne
         front_token();
         if(token2.stav != S_SEMICOLON)
            return SYNTAX_ERR;
      }
      else if(token2.stav == S_LZAT)    //lubovalna ina funckia
      {
         if(i > 0)
         {
            error = build_function_call_scnd(i);
            if(error != SUCCESS)
               return error;
         }
         else 
         {
            /***********VYRAZ**************/
            front_token();
            if(token2.stav != S_PZAT)
               return SYNTAX_ERR;

            front_token();
            if(token2.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
      }
      else
         return SYNTAX_ERR;
   }
   else if(!((strcmp(token2.data, "String"))&&(strcmp(token2.data, "int"))&&(strcmp(token2.data, "double"))))  //pravidlo <SL> -> <PARS> <VD>
   {  
      front_token();
      if(token2.stav != S_ID)
         return SYNTAX_ERR;

      front_token();
      if(token2.stav == S_PRIR)
      {
         /**********************ZAVOLAT OVERENIE VYRAZU***************************/
         front_token();
         if(token2.stav != S_SEMICOLON)
            return SYNTAX_ERR;
      }
      else if(token2.stav == S_SEMICOLON)
         ;
      else
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token2.data, "return")))
   {
         //zatial takto inak tu bude vyraz      
         front_token();
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
      fprintf(stderr, "Syntax error, unexpeted token \"%s\"\n.", token2.data);
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
      /********************************VYRAZ*********************************/

      front_token();   //cakam )
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
      /****************VYHODNOTENIE VYRAZU*******************/

      front_token();
      if(token2.stav != S_INT)
         return FILIP_ERR;

      /*************************ZATIAL INT FAKE**************/
      front_token();
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
      int i = is_build_function_scnd();

      front_token();

      if(token2.stav == S_PRIR)    //priradenie do premennej
      {  
         //provizorne
         front_token();
         if(token2.stav != S_SEMICOLON)
            return SYNTAX_ERR;
      }
      else if(token2.stav == S_LZAT)    //lubovalna ina funckia
      {
         if(i > 0)
         {
            error = build_function_call_scnd(i);
            if(error != SUCCESS)
               return error;
         }
         else 
         {
            /***********VYRAZ**************/
            front_token();
            if(token2.stav != S_PZAT)
               return SYNTAX_ERR;

            front_token();
            if(token2.stav != S_SEMICOLON)
               return SYNTAX_ERR;
         }
      }
      else
         return SYNTAX_ERR;
   }
   else if(!((strcmp(token2.data, "String"))&&(strcmp(token2.data, "int"))&&(strcmp(token2.data, "double"))))  //pravidlo <SL> -> <PARS> <VD>
   {  
         return SYNTAX_ERR;
   }
   else if(!(strcmp(token2.data, "return")))
   {
         //zatial takto inak tu bude vyraz      
         front_token();
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
      fprintf(stderr, "Syntax error, unexpeted token \"%s\"\n.", token2.data);
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
            //overenie v TS ci je dany ID string
             break;
         default: return SEMANTIC_PROG_ERR;
      }  

         return SYNTAX_ERR;
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
            //overenie v TS ci je dany ID string
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
            //overenie v TS ci je dany ID string
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
            //overenie v TS ci je dany ID string
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