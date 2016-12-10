#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ial.h"
#include "buildin.h"
#include "cleaner.h"

// v interprete uvolnovat vystup str_ret
//pridany mymalloc
//Radenie retazca pomocou algoritmu heapsort
char* sort(const char *str)
{
    unsigned int n = strlen(str);
    char *str_tmp=NULL;
    if ((str_tmp=(char*)mymalloc(sizeof(char)*(n+2))) == NULL )
        return NULL;

    strncpy(&str_tmp[1],str,n);
    str_tmp[n+1]='\0';
    str_tmp[0]='#';

    return heapsort(str_tmp,n);
}


//vstavana funkcia na hladanie podretazcov
char* substring(const char *s, const int i, const int n){
    if (s == NULL){
        return NULL;
    }

    if((i) < 0 || i >= (int)strlen(s)){
        return NULL;
    }

    if(i+n > (int)strlen(s)){
        return NULL;
    }



    char* tmp;
    if ((tmp = (char *) mymalloc(n+1 * sizeof(char))) == NULL){
        return NULL;
    }
    int index=0;

    for(int k=i; k<i+n; k++){
        tmp[index++]=s[k];
    }

    tmp[n]='\0';


    return tmp;

}


//vstavana funkcia na nacitanie Stringov
void readString(symData * dest)
{
    int chunk=10;
    int c;

    char *save1=malloc(sizeof(char)*chunk);
    int counter=0;
    int reallocs=1;

    while ((c=getchar()) != '\n')
    {
        if (counter == (reallocs*chunk)-1)
        {
            reallocs++;
            save1=realloc(save1,reallocs*chunk);
        }
        *(save1+(counter))=c;
        counter++;
    }
    *(save1+counter)='\0';

    if(dest != NULL){
        if(dest->init == true && dest->funcdata_union.offset != -1){
            free(dest->ptr_union.str);
        }
        if(dest->funcdata_union.offset == -1){
            dest->ptr_union.str=mymalloc(sizeof(char) * strlen(save1) + 1);
        }
        else{
            dest->ptr_union.str=malloc(sizeof(char) * strlen(save1) + 1);
        }
        if(dest->ptr_union.str == NULL){
            exit(99);
        }
        dest->init=true;
        strcpy(dest->ptr_union.str, save1);
    }
    free(save1);
}
//Vstavana funkcia na nacitanie celych cisel
int readInt(symData * dest)
{
    int i=0;
    int chunk=10;
    int c;
    char * error;

    char *save1=malloc(sizeof(char)*chunk);
    int counter=0;
    int reallocs=1;

    while ((c=getchar()) != '\n')
    {
        if (counter == (reallocs*chunk)-1)
        {
            reallocs++;
            save1=realloc(save1,reallocs*chunk);
        }
        *(save1+(counter))=c;
        counter++;
    }
    *(save1+counter)='\0';

    i = strtol(save1, &error, 10);
    if(*error == 0 || *error== 32 || *error == 10){
        if(dest != NULL){
            if(dest->type==tInt){
                dest->init= true;
                dest->ptr_union.i = i;
            }
            else if(dest->type==tDouble){
                dest->init= true;
                dest->ptr_union.d = i;
            }
            else{
                free(save1);
                return 4;
            }
        }
    }
    else{
        free(save1);
        return 7;
    }

    free(save1);
    return 0;
}
//Vstavana funkcia na nacitanie desatinnych cisel
int readDouble(symData * dest)
{
    double d=0.0;
    int chunk=10;
    int c;
    char * error;

    char *save1=malloc(sizeof(char)*chunk);
    int counter=0;
    int reallocs=1;

    while ((c=getchar()) != '\n')
    {
        if (counter == (reallocs*chunk)-1)
        {
            reallocs++;
            save1=realloc(save1,reallocs*chunk);
        }
        *(save1+(counter))=c;
        counter++;
    }
    *(save1+counter)='\0';

    d = strtod(save1, &error);
    if(*error == 0 || *error== 32 || *error == 10){
        if(dest != NULL){
            if(dest->type==tInt){
                dest->init= true;
                dest->ptr_union.i = d;
            }
            else if(dest->type==tDouble){
                dest->init= true;
                dest->ptr_union.d = d;
            }
            else{
                free(save1);
                return 4;
            }
        }
    }
    else{
        free(save1);
        return 7;
    }

    free(save1);
    return 0;
}
//Vstavana funkcia na vyhladanie indexu, kde zacina v retazci zadany pattern
int find(char* s,char* search)
{
    int s_len=strlen(s);
    int search_len=strlen(search);
    int found;

    int match_jump[search_len];
    int char_jump[MAXCHAR];

    if (s_len < search_len)
        return -1;

    if (s_len == search_len)
    {
        if(strcmp(s,search)==0)
            return 0;
        else
            return -1;
    }

    if (search_len == 1)
    {
        for(int i=0;i<s_len;i++)
            if (search[0] == s[i])
                return i;

        return -1;
    }

    if (search_len == 0)
        return 0;

    found=(boyer_moore(search,s,match_jump,char_jump)-1);

    if (found == s_len)
        return -1;
    else
        return found;
}

