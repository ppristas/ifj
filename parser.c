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
#include "parser.h"
#include "scaner.h"
#include "error.h"

tStack p_stack;

bool fMain = false;  //bola uz najdena class Main? true = ano, false = nie
bool fRun = false;   //bola uz najdena funkcia run v classe Main?

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

int parser()
{
   error = SUCCESS;
   stackInit(&p_stack);  //inicializacia stacku pouzivaneho pri prediktivnej SA 
   stackPush(&p_stack, T_dolar);
   
   stackPush(&p_stack, N_P);  //zaciatocny neterminal P (prog)
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

   if((fMain == false)&&(token.stav == S_EOF))  //chyba, prazdny subor
   {
      error = SYNTAX_ERR;
      return error;
   }
   else if((fMain == true)&&(token.stav == S_EOF))   //pravidlo P -> epsilon
   {
      expand(&p_stack,1,T_EOF);
      if(p_stack.top->data == T_EOF && token.stav == S_EOF)
         stackPop(&p_stack);
      if(p_stack.top->data == T_dolar)          //vsetky tokeny boli precitane, dosiel token konca suboru
         return SUCCESS;                        //vstup je akceptovany
      else                                      //na zasobniku este nieco ostalo, vsetko nebolo pokryte, error
         return SYNTAX_ERR;
   }
   if(!(strcmp(token.data, "class")))                   //pravidlo P -> C P
   {
      expand(&p_stack, 2, N_P, N_C);
      error = class();
   }
   else
   { 
      return SYNTAX_ERR;
   }
   get_token();
   if(!(strcmp(token.data, "class"))) 
   {
      error = prog();
   }

   get_token();
   if(token.stav != S_EOF)
      error = SYNTAX_ERR;

   return error;
}

//Pravidlo:    C -> class idc { <AC> }

int class()
{
   if(error != SUCCESS)
      return error;
   expand(&p_stack,5, T_P_KOSZ, N_AC, T_L_KOSZ, T_id, T_class);   //pravidlo C -> class id { AC }         
   if(p_stack.top->data != T_class)
      return FILIP_ERR;
   stackPop(&p_stack);    //token.data = p_stack.top, mozeme pouzit porovnavacie pravidlo
   get_token();   //ocakavam id
   if((token.stav != S_ID) || ((strcmp(token.data, "Main"))&&(!fMain))) //Main musi byt prvy class
      return SYNTAX_ERR;
   if(!(strcmp(token.data, "Main")))
      fMain = true;
   if(p_stack.top->data != T_id)
      return SYNTAX_ERR;
   stackPop(&p_stack);

   get_token();   //ocakavam {
   if(token.stav != S_L_KOSZ)
      return SYNTAX_ERR;
   stackPop(&p_stack);    //vrchol je {, token { rovnako

   get_token();
   error = after_class();
   if(error != SUCCESS)
      return error;

   get_token();   //ocakavam }
   

   if(error != SUCCESS)
      return error;
               
   if(token.stav != S_P_KOSZ)
      return SYNTAX_ERR;
   /*if(p_stack.top->data != T_P_KOSZ)
      return FILIP_ERR;*/

   stackPop(&p_stack); //token = vrchol, mozeme popnut
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

   expand(&p_stack, 3, N_AC, N_SD, T_static); //token = static, vrchol = static
   stackPop(&p_stack); //popujeme static zo stacku
                //ak pride void pravidlo: <SD> -> void id <SDA>      
   get_token(); //inak pravidlo: <SD> -> <PARS> <DECL>;
   if(!(strcmp(token.data, "void")))
   {
      expand(&p_stack, 3, N_SDA, T_id, T_void);
      stackPop(&p_stack);  //na vrchu je void, token je void -> pop
      get_token();   //ocakavam id
      if(token.stav != S_ID)
         return SYNTAX_ERR;
      stackPop(&p_stack);  //vrchol je id -> pop, ostalo na vrchole <SDA>
      expand(&p_stack, 6, T_P_KOSZ, N_MB, T_L_KOSZ, T_PZAT, N_PA, T_LZAT);
      //pouzite pravidlo <SDA> -> ( <PA> ) { <MB> }
      get_token(); //musi byt lava zatvorka
      if(token.stav != S_LZAT)
         return SYNTAX_ERR;
      stackPop(&p_stack);  //pop (, top = <PA>
      get_token();   //ocakava bud ) v pripade prazndych argumentov, alebo type
   

      if(token.stav == S_PZAT)   //prazdny pocet argumentov
      {
         stackPop(&p_stack);  //pop <PA>, pravidlo: <PA> -> epsilon
         stackPop(&p_stack);  //pop )
         get_token();   
         if(token.stav != S_L_KOSZ)
            return SYNTAX_ERR;
         stackPop(&p_stack);  //pop {, top = <MB>
         
         error = main_body();

         get_token(); //cakam }
         if(token.stav != S_P_KOSZ)
            return SYNTAX_ERR;

         /*get_token();
         if(!(strcmp(token.data, "static"))) //nachadza sa za telom funckie este nieco static?
            error = after_class();*/
         return error;

      }  
      else if(token.stav == S_KEY)
      {
         if((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double")))
            return SYNTAX_ERR;

         error = params_after(); //riadenie predane params_after  
                                 
         if(error != SUCCESS)
            return error;

         if(p_stack.top->data != T_PZAT)
            return FILIP_ERR;
         if(token.stav != S_PZAT)
            return FILIP_ERR;

         stackPop(&p_stack); //na vrchole je )
         get_token();   //musi byt {
         if(token.stav != S_L_KOSZ)
            return SYNTAX_ERR;
         stackPop(&p_stack);  //token = {, stack = { -> pop
         
         error = main_body();

         get_token();   //cakam kosatu zavorku
         if(token.stav != S_P_KOSZ)
            return SYNTAX_ERR;

         /*get_token();
         if(!(strcmp(token.data, "static"))) //nachadza sa za telom funckie este nieco static?
            error = after_class();
         ---------------------------------------
         printf("tuuuuuuuuuuu2\n");
         printf("%s\n", token.data);
         printf("%d\n", error);
         ---------------------------------------*/
         return error;
      }             
   }
   else if(!((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double"))))  //pravidlo <SD> -> <Pars> <Decl>
   {
      expand(&p_stack, 3, N_decl, T_id, N_type);   //<Pars> nahradim <type> id
      stackPop(&p_stack);  //token je type (string, int alebo double) , top = type, popujem
      get_token();
      if(token.stav != S_ID)
         return SYNTAX_ERR;
      stackPop(&p_stack);               
      get_token();


      switch(token.stav)
      {
         case S_SEMICOLON:          //pravidlo <Decl> -> ;
            stackPop(&p_stack);     //pop decl
            get_token();
            if(!(strcmp(token.data, "static")))
            {
               error = after_class();
            }
            return error;
            break;

         case S_PRIR:               // pravidlo <Decl> ->
            /* --------------EXPRESION ESTE NEVIEM----------------- */
            stackPop(&p_stack);
            return error;
            break;

         case S_LZAT:               // pravidlo <Decl> -> ( <PA> ) { <MB> }
            expand(&p_stack,6, T_P_KOSZ, N_MB, T_L_KOSZ, T_PZAT, N_PA, T_LZAT);
            stackPop(&p_stack);  //akceptovana zavorka
            /*------------------------TU COPY--------------------*/
            get_token();   //ocakava bud ) v pripade prazndych argumentov, alebo type
                     
            if(token.stav == S_PZAT)   //prazdny pocet argumentov
            {
               stackPop(&p_stack);  //pop <PA>, pravidlo: <PA> -> epsilon
               stackPop(&p_stack);  //pop )
               get_token();   
               if(token.stav != S_L_KOSZ)
                  return SYNTAX_ERR;
               stackPop(&p_stack);  //pop {, top = <MB>
               
               error = main_body();

               get_token(); //cakam }
               if(token.stav != S_P_KOSZ)
                  return SYNTAX_ERR;   ////////////////////////tu neskacem hore rekurzivne

               return error;

            }  
            else if(token.stav == S_KEY)
            {
               if((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double")))
                  return SYNTAX_ERR;

               error = params_after(); //riadenie predane params_after  
                                       
               if(error != SUCCESS)
                  return error;

               if(p_stack.top->data != T_PZAT)
                  return FILIP_ERR;
               if(token.stav != S_PZAT)
                  return FILIP_ERR;

               stackPop(&p_stack); //na vrchole je )
               get_token();   //musi byt {
               if(token.stav != S_L_KOSZ)
                  return SYNTAX_ERR;
               stackPop(&p_stack);  //token = {, stack = { -> pop
               
               error = main_body();

               get_token();
               if(token.stav != S_P_KOSZ)
                  return SYNTAX_ERR;
               
               /*get_token();
               if(!(strcmp(token.data, "static"))) //nachadza sa za telom funckie este nieco static?
                  error = after_class();*/
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
   expand(&p_stack, 3, N_NP, T_id, N_type);  //pravidlo PA -> <pars> <NP> upravene na PA -> <type> <id> <NP>
   stackPop(&p_stack);  //top = type, token = type
   get_token();   //cakam id
   if(token.stav != S_ID)
      return SYNTAX_ERR;
         /*------------------------------------------------------------------------------------------------------------------------*/
         printf("tuuuuuuuuuuu\n");
         printf("%s\n", token.data);
         printf("%d\n", error);
         /*------------------------------------------------------------------------------------------------------------------------------------*/
   stackPop(&p_stack); //na tope bol id, teraz je <NP>
   get_token();   //ak ) koncime a predavame riadenie, ak , pokracujeme
   if(token.stav == S_PZAT)
   {
      stackPop(&p_stack); //popneme z vrcholu <NP> pravidlom <NP> -> epsilon;
      return error;
   }
   else if(token.stav == S_CIARKA)
   {
      get_token(); //cakam typ
      if((strcmp(token.data, "String"))&&(strcmp(token.data, "int"))&&(strcmp(token.data, "double")))
            return SYNTAX_ERR;
      error = params_after();
   }
   else           // inak error
      return SYNTAX_ERR;
   return error; //sem by sme sa nemali nikdy dostat
}

int main_body()
{
   return SUCCESS;
}
