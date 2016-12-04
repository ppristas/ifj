#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ial.h"
#include "buildin.h"
#include "cleaner.h"

// v interprete uvolnovat vystup str_ret
//pridany mymalloc
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

//Dlzka retazca
int length(const char* s){
    int i;
    for(i=0; s[i] != '\0'; i++){}
    return i;
}


// tmp treba uvolnit
char* substring(const char *s, const int i, const int n){
    if(i < 0 || i >= length(s)){
        return NULL;
    }

    if(i+n > length(s)){
        return NULL;
    }

    if (s == NULL){
        return NULL;
    }


    char* tmp;
    if ((tmp = (char *) malloc(n+1 * sizeof(char))) == NULL){
        return NULL;
    }
    int index=0;

    for(int k=i; k<i+n; k++){
        tmp[index++]=s[k];
    }

    tmp[n]='\0';


    return tmp;

}



int compare(const char *s1, const char *s2){
    int index=0;
    if(length(s1) <= length(s2)){
        while(s1[index] != '\0'){
            if(s1[index] != s2[index]){
                return s1[index] - s2[index];
            }
            index++;
        }
        if(s1[index] == '\0' && s2[index] != '\0'){
        return length(s1)-length(s2);
        }
    }

    else if(length(s2) < length(s1)){
        while(s2[index] != '\0'){
            if(s2[index] != s1[index]){
                return s1[index] - s2[index];
            }
            index++;
        }
        if(s2[index] == '\0' && s1[index] != '\0'){
        return length(s1)-length(s2);
        }

    }

    return 0;
}

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

