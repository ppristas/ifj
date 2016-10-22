/********************************************************************************
   *    Predmet:        Formalne jazyky a prekladace (IFJ), Algoritmy (IAL)     *
   *    Projekt:        Interpret zjednoduseneho jazyka Java SE                 *
   *    Variant:        b/2/II                                                  *
   *    Tim:            092                                                     *
   *    Subor:          predec.c                                                *
   *    Riesitelia:     Filip Mik       (xmikfi00@stud.fit.vutbr.cz)            *
   *                    Peter Ziska     (xziska02@stud.fit.vutbr.cz)            *
   *                    Peter Pristas   (xprist05@stud.fit.vutbr.cz)            *
   *                    Daniel Florek   (xflore02@stud.fit.vutbr.cz)            *
   *                    Martin Grnac    (xgrnac00@stud.fit.vutbr.cz)            *
   *                                                                            *               
   ********************************************************************************/

   #include <stdio.h>
   #include <stdbool.h>
   #include "error.h"

   #define TAB_SIZE (17)

   static const char predenense_table[TAB_SIZE][TAB_SIZE] = 
   {/*     +    -    *    /    (    )   id   idf   =    <    >   <=   >=   ==   !=    ,    $  */
/* +  */ {'>', '>', '<', '<', '<', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'}   
/* -  */ {'>', '>', '<', '<', '<', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'}   
/* *  */ {'>', '>', '>', '>', '<', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'}  
/* /  */ {'>', '>', '>', '>', '<', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'}  
/* (  */ {'<', '<', '<', '<', '<', '=', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}   
/* )  */ {'>', '>', '>', '>', ' ', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'} 
/*id  */ {'>', '>', '>', '>', ' ', '>', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'} 
/*idf */ {' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '} 
/* =  */ {'<', '<', '<', '<', '<', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'}
/* <  */ {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'} 
/* >  */ {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'} 
/* <= */ {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'} 
/* >= */ {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'} 
/* == */ {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'} 
/* != */ {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>'} 
/* ,  */ {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '} 
/* $  */ {'<', '<', '<', '<', '<', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}  
      }