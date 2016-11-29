#include <stdio.h>
#include <string.h>

#include "temp_tab.h"




void tmp_htab_init(TMP_HTAB* table)
{
    for (int i=0; i<TMP_HTAB_SIZE; i++)
    {
        (*table)[i]=NULL;
    }
}


static int tmp_hash_key(char *key)
{
    unsigned long hash = 5381;
    int c;

     while (c = *key++)
         hash = ((hash << 5) + hash) + c;

     return (hash % TMP_HTAB_SIZE);
}


T_TMP_ITEM* tmp_htab_search(TMP_HTAB* table,char* key)
{
    int index=tmp_hash_key(key);
    T_TMP_ITEM* tmp= (*table)[index];

    while (tmp != NULL)
    {
        if (strcmp(tmp->key,key) == 0)
            return tmp;

        tmp=tmp->next;
    }
    return NULL;
}

// Najde a vrati data hodi sa ako parameter do generatora instrukcii
symData* tmp_get_data(TMP_HTAB *table, char *key)
{
    T_TMP_ITEM* item= tmp_htab_search(table,key);

    if (item != NULL)
        return item->data;

    return NULL;
}

void tmp_htab_insert(TMP_HTAB* table,symbolType type,void* ptr,char* key)
{
    if (key != NULL)
    {
        T_TMP_ITEM* tmp= tmp_htab_search(table,key);

        if (tmp == NULL)
        {
            int index= tmp_hash_key(key);

            tmp= (T_TMP_ITEM*)mymalloc(sizeof(T_TMP_ITEM));

            tmp->data =(symData*)mymalloc(sizeof(symData));
            tmp->key=(char*)mymalloc(sizeof(char) * ((int)strlen(key) + 1));
            strcpy(tmp->key,key);

            tmp->data->type=type;

            if (type == tInt)
            {
                tmp->data->ptr_union.i=(*(int*)ptr);
            }
            else if (type == tDouble )
            {
                tmp->data->ptr_union.d=(*(double*)ptr);
            }
            else if (type == tString )
            {
                tmp->data->ptr_union.str=(char*)mymalloc(sizeof(char)*(int)(strlen((char*)ptr)+1));
                strcpy(tmp->data->ptr_union.str,((char*)ptr));
            }

            tmp->next=(*table)[index];
            (*table)[index]=tmp;

        }
    }
}


static void Print_table_row(T_TMP_ITEM* item)
{
    T_TMP_ITEM* tmp=item;

    while (tmp != NULL)
    {
        if (tmp->data->type == tInt)
        {
            printf("(%s){ type = INTEGER, data = %d }",tmp->key,tmp->data->ptr_union.i);
        }
        else if(tmp->data->type == tDouble)
        {
            printf("(%s){ type = DOUBLE, data = %f }",tmp->key,tmp->data->ptr_union.d);
        }
        else if(tmp->data->type == tString)
        {
            printf("(%s){ type = STRING, data = %s }",tmp->key,tmp->data->ptr_union.str);
        }

        tmp=tmp->next;
    }
}


static void Print_table(TMP_HTAB* tabulka)
{
    for (int i=0;i<TMP_HTAB_SIZE;++i)
    {
        Print_table_row(((*tabulka)[i]));

        if ((*tabulka)[i] != NULL)
        {
            printf("\n");
        }

    }
}

// tabulka pre medzi vysledky  allokovat v maine;
//TMP_HTAB* middle_vysledky = (TMP_HTAB*)mymalloc(sizeof(TMP_HTAB));

//tabulka pre konstanty
//TMP_HTAB* const_table = (TMP_HTAB*)mymalloc(sizeof(TMP_HTAB)); 

TMP_HTAB* middle_vysledky;
TMP_HTAB* const_table;


/*
int main(int argc, char *argv[])
{
    initCleaner();

    TMP_HTAB* tabulka = (TMP_HTAB*)mymalloc(sizeof(TMP_HTAB));


    tmp_htab_init(tabulka);

    int i;
    double dbl;

    i=30;
    tmp_htab_insert(tabulka,tInt,&i,"T1");
    i=33;
    tmp_htab_insert(tabulka,tInt,&i,"T2");
    i=35;
    tmp_htab_insert(tabulka,tInt,&i,"T3");
    i=80;
    tmp_htab_insert(tabulka,tInt,&i,"T4");
    i=300;
    tmp_htab_insert(tabulka,tInt,&i,"T5");

    dbl=300.45;
    tmp_htab_insert(tabulka,tDouble,&dbl,"T6");
    dbl=0.45;
    tmp_htab_insert(tabulka,tDouble,&dbl,"T7");
    dbl=1300.405;
    tmp_htab_insert(tabulka,tDouble,&dbl,"T8");
    dbl=30120.333;
    tmp_htab_insert(tabulka,tDouble,&dbl,"T9");

    tmp_htab_insert(tabulka,tString,"Abcndsd","T10");
    tmp_htab_insert(tabulka,tString,"KKSSSSSSS","T151");
    tmp_htab_insert(tabulka,tString,"DSAA_DSAD_DSA","T48");
    tmp_htab_insert(tabulka,tString,"S__DSD___SSSS_","T64");
    tmp_htab_insert(tabulka,tString,"LDS_THC","T65");

    Print_table(tabulka);


    clearAll();



    return 0;
}
*/