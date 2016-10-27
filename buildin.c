#include "ial.h"
#include "buildin.h"

// v interprete uvolnovat vystup str_ret 
char* sort(const char *str)
{
    unsigned int n = strlen(str);

    char *str_tmp=NULL;
    char *str_ret=NULL;
    if ((str_ret=malloc(sizeof(char)*(n+1))) == NULL)
        return NULL;

    if ((str_tmp=malloc(sizeof(char)*(n+2))) == NULL )
        return NULL;

    strncpy(&str_tmp[1],str,n);
    str_tmp[n+1]='\0';
    str_tmp[0]='#';

    memset(str_ret,0,n+1);

    heapsort(str_tmp,str_ret,n);

    free(str_tmp);
    return str_ret;
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

