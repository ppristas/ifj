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

#include <stdarg.h>
#include "stack.h"
#include "parser.h"
#include "scaner.h"
#include "error.h"

tStack p_stack;

bool fMain = false;  //bola uz najdena class Main? true = ano, false = nie

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

int parser()
{
   error = SUCCESS;
   get_token();
   error = prog();
      
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
   if(!(strcmp(token.data, "Main")))
      fMain = true;
   if((token.stav != S_ID) || ((strcmp(token.data, "Main"))&&(!fMain))) //Main musi byt prvy class
      return SEMANTIC_PROG_ERR;
   if(!(strcmp(token.data, "ifj16")))  //class ifj 16 nemoze byt definovany
      return SEMANTIC_PROG_ERR;

   get_token();   //ocakavam {
   if(token.stav != S_L_KOSZ)
      return SYNTAX_ERR;
   stackPop(&p_stack);    //vrchol je {, token { rovnako

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
      else if(token.stav == S_KEY)
      {
         if((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double")))
            return SYNTAX_ERR;

         error = params_after(); //riadenie predane params_after  
                                 
         if(error != SUCCESS)
            return error;

         if(token.stav != S_PZAT)
            return FILIP_ERR;

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
            /* --------------EXPRESION ESTE NEVIEM----------------- */

            get_token();   //za priradenim ;
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
                  return FILIP_ERR;

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

   if(!(strcmp(token.data, "ifj16")))  //skumany token bol nacitany pred volanim main_body
   {  
      get_token();
      if(strcmp(token.data, "print"))
      {   
         error = build_function_call();  
         if(error != SUCCESS)
            return error;
      }
      else if(!(strcmp(token.data, "print")))
      {
         error =  build_print();
         if(error != SUCCESS)
            return error;
      }
      else
         return SYNTAX_ERR;
      if(error != SUCCESS)
         return error;
   }
   else if(!(strcmp(token.data, "while")))  //pravidlo <SL> -> while ( <E> ) { main body }
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

      error = main_body();    // v tele whilu moze byt hocico, preto volame main_body
      
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
      error = main_body();
     
      if(error != SUCCESS)
         return error;

      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
      get_token();

      if(strcmp(token.data, "else"))   /*******************************************************************NEFUNGUJE PRAZDNY ELSE************************************************/
         return SYNTAX_ERR;      

      get_token();
      if(token.stav != S_L_KOSZ)
         return SYNTAX_ERR;

      get_token();
      if(token.stav != S_P_KOSZ)
         error = main_body();

      if(error != SUCCESS)
         return error;
      if(token.stav != S_P_KOSZ)
         return SYNTAX_ERR;
   }
   else if(token.stav == S_ID)
   {
      get_token();
      if(token.stav == S_PRIR)
      {

      }
      else if(token.stav == S_LZAT)
      {

      }
      else
         return SYNTAX_ERR;
   }

   if(error != SUCCESS)
      return error;

   get_token();
  
   if(token.stav != S_P_KOSZ)
      error = main_body();

   if(error != SUCCESS)
      return error;

   if(token.stav == S_P_KOSZ)
      return error;   
   return error;
}

int build_function_call()
{
   if(error != SUCCESS)
      return error;
   if(!(strcmp(token.data, "readInt") && strcmp(token.data, "readDouble") && strcmp(token.data, "readString")))    //pravidlo <SL> -> <FC> -> read...();
   {
      get_token();   //musi byt (
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
   else if(!(strcmp(token.data, "sort") && strcmp(token.data, "length")))
   {
      get_token();   //musi byt (
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      /************TU MUSI BYT SPRACOVANIE VYRAZU*******************/

         return SYNTAX_ERR;
      get_token();   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR; 
      return error;                       
   }
   else if(!(strcmp(token.data, "find") && strcmp(token.data, "compare")))
   {
      get_token();   //musi byt (
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      /************TU MUSI BYT SPRACOVANIE VYRAZU*******************/

      get_token();
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      /************TU MUSI BYT SPRACOVANIE VYRAZU*******************/

      get_token();   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR;
      return error;    
   }
   else if(!(strcmp(token.data, "substr")))
   {
      get_token();   //musi byt (
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;

      /************TU MUSI BYT SPRACOVANIE VYRAZU*******************/

      get_token();
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      /************TU MUSI BYT SPRACOVANIE VYRAZU*******************/

      get_token();
      if(token.stav != S_CIARKA) //musi byt ciarka medzi argumentami
         return SYNTAX_ERR;

      /************TU MUSI BYT SPRACOVANIE VYRAZU*******************/

      get_token();   //musi byt )
      if(token.stav != S_PZAT)
         return SYNTAX_ERR;
      get_token();   //musi byt ;
      if(token.stav != S_SEMICOLON)
         return SYNTAX_ERR;
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
   get_token();
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


   /*******************SPRACOVANIE VYRAZU***************/

   get_token();
   if(token.stav != S_INT)
      return SYNTAX_ERR;

   /*****************ZATIAL FAKE, LEN INT***************/

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