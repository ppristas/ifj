#include <stdio.h>
#include "String.h"
#include <stdlib.h>

#define ALLOC_VAL 16


//Inicializacia struktury na 0 znakov a alokovanie prvej sekvencie 16 bajtov;
int string_init(string *s){
    if((s->str = (char *) malloc(ALLOC_VAL*sizeof(char))) == NULL){
        return 0;
    }
    s->length=0;
    s->allocated = ALLOC_VAL;
    return 1;
}

//uvolnovanie pamate
void string_free(string *s){
    free(s->str);
}

//naplnanie stringu v pripade potreby aj realokacia
void string_fill(string *s, char c){

    if(s->length == s->allocated && s->length){
        string_realloc(s);
        s->str[s->length] = c;
        s->length++;
    }

    else{
        s->str[s->length] = c;
        s->length++;
    }
}

//realokacia stringu o dalsich 16 bajtov
int string_realloc(string *s){
    s->str=realloc(s->str, s->length + ALLOC_VAL * sizeof(char));
    s->allocated+=ALLOC_VAL;
    return 1;
}

//vypisova funkcia
void string_print(string *s){
    for(int i=0; i<s->length; i++){
        printf("%c", s->str[i]);
    }
}

//spoji dva retazce do toho prveho
int string_merge(string *s1, string *s2){
    for(int i=0; i<s2->length; i++){
        string_fill(s1, s2->str[i]);
    }

}

void string_delChar(string *s){
    s->str[s->length]='\0';
    s->length--;
}
