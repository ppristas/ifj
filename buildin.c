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
