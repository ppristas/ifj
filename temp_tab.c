#include <stdio.h>
#include <string.h>

#include "stack_frame.h"
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

     while ((c = *key++) == true)
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

symData* tmp_get_data(TMP_HTAB *table, char *key)
{
    T_TMP_ITEM* item= tmp_htab_search(table,key);

    if (item != NULL)
        return item->data;

    return NULL;
}


void tmp_htab_insert(TMP_HTAB* table,symbolType type,void* ptr,char* key,int offset)
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

            tmp->data->funcdata_union.offset=offset;


            if (ptr == NULL)
            {
                if (type == tInt)
                {
                    tmp->data->ptr_union.i=0;
                }
                else if (type == tDouble )
                {
                    tmp->data->ptr_union.d=0.0;
                }
                else if (type == tString )
                {
                    tmp->data->ptr_union.str=NULL;
                }
            }
            else
            {
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
            }

            tmp->next=(*table)[index];
            (*table)[index]=tmp;

        }
    }
}



void Print_table_row(T_TMP_ITEM* item)
{
    T_TMP_ITEM* tmp=item;

    while (tmp != NULL)
    {
        if (tmp->data->type == tInt)
        {
            printf("(%s){ type = INTEGER, data = %d }::[%d]",tmp->key,tmp->data->ptr_union.i,tmp->data->funcdata_union.offset);
        }
        else if(tmp->data->type == tDouble)
        {
            printf("(%s){ type = DOUBLE, data = %f }::[%d]",tmp->key,tmp->data->ptr_union.d,tmp->data->funcdata_union.offset);
        }
        else if(tmp->data->type == tString)
        {
            printf("(%s){ type = STRING, data = %s }::[%d]",tmp->key,tmp->data->ptr_union.str,tmp->data->funcdata_union.offset);
        }

        tmp=tmp->next;
    }
}


void Print_table(TMP_HTAB* tabulka)
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
/*
static symData* fill_data(T_TMP_ITEM* data,char* key,int count)
{
    data->key=(char*)mymalloc(sizeof(char)* ((int)strlen(key) +1));

    data->data=(symData*)mymalloc(sizeof(symData));
    data->data->funcdata_union.var_count=count;

    return data->data;
}

TMP_HTAB* tabulka;

int main(int argc, char *argv[])
{

    initCleaner();

    stackFrame_t* Stack=NULL;

    stack_frame_create(&Stack);
    stack_frame_init(Stack);

    tabulka = (TMP_HTAB*)mymalloc(sizeof(TMP_HTAB));


    tmp_htab_init(tabulka);

    int i;
    double dbl;

    i=30;
    tmp_htab_insert(tabulka,tInt,&i,"T1",0);
    i=33;
    tmp_htab_insert(tabulka,tString,"LDS_THC","T2",2);
    tmp_htab_insert(tabulka,tString,"S__DSD___SSSS_","T3",1);
    tmp_htab_insert(tabulka,tString,"LDS_THC","T4",3);
    char* string ="PICAAAAAAAA";
    tmp_htab_insert(tabulka,tString,string,"T5",4);

//create frame
    T_TMP_ITEM func1;
    stack_frame_push(Stack,fill_data(&func1,"funkcia1",5));

    frame_fill_variable(Stack,tmp_get_data(tabulka,"T1"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"T5"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"T3"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"T4"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"T2"));





    T_TMP_ITEM func2;
    stack_frame_push(Stack,fill_data(&func2,"funkcia2",3));

    frame_fill_variable(Stack,tmp_get_data(tabulka,"T2"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"T1"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"T3"));

    symData* addres=decode_addres(Stack,2);
    strcpy(addres->ptr_union.str,"XXPorn");


    //-------------------------------------------------------------

    i=20;
    tmp_htab_insert(tabulka,tInt,&i,"prem1",0);
    i=55;
    tmp_htab_insert(tabulka,tInt,&i,"prem2",1);
    dbl=0.98;
    tmp_htab_insert(tabulka,tDouble,&dbl,"prem3",2);
    i=5;
    tmp_htab_insert(tabulka,tInt,&i,"prem4",3);
    dbl=89.0;
    tmp_htab_insert(tabulka,tDouble,&dbl,"prem5",4);


    T_TMP_ITEM func3;
    stack_frame_push(Stack,fill_data(&func3,"funkcia2",5));


    frame_fill_variable(Stack,tmp_get_data(tabulka,"prem2"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"prem1"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"prem3"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"prem5"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"prem4"));

   // addres=decode_addres(Stack,4);


    //do func4 dame int prem1 int prem2 double prem3
    // func4(int a,int b,double c);

    i=510;
    tmp_htab_insert(tabulka,tInt,&i,"x",3);
    dbl=4.2;
    tmp_htab_insert(tabulka,tDouble,&dbl,"y",4);
    dbl=5.5;
    tmp_htab_insert(tabulka,tDouble,&dbl,"z",5);

    T_TMP_ITEM func4;
    // ocislovane addresy parametrov 0 1 2
    //lokalne premenne pre func4  int x,double y, double z
    stack_frame_push(Stack,fill_data(&func4,"funkcia3",6));

    //do func4 dame int prem1 int prem2 double prem3
    //pushujeme na zasobnik

    printf("Pushujem\n");
    arg_push(Stack,tmp_get_data(tabulka,"prem1"));
    arg_push(Stack,tmp_get_data(tabulka,"prem2"));
    arg_push(Stack,tmp_get_data(tabulka,"prem3"));

    frame_fill_variable(Stack,tmp_get_data(tabulka,"x"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"y"));
    frame_fill_variable(Stack,tmp_get_data(tabulka,"z"));



   Print_addres(addres);

   strcpy(addres->ptr_union.str,"kokoto");

   addres=decode_addres(Stack,1);

   strcpy(addres->ptr_union.str,"Danootoo");

   addres=pre_decode_addres(Stack,2);

   Print_addres(addres);



    Print_stack(Stack);



    dbl=300.45;
    tmp_htab_insert(tabulka,tDouble,&dbl,"T6",0);
    dbl=0.45;
    tmp_htab_insert(tabulka,tDouble,&dbl,"T7",1);
    dbl=1300.405;
    tmp_htab_insert(tabulka,tDouble,&dbl,"T8",2);
    dbl=30120.333;
    tmp_htab_insert(tabulka,tDouble,&dbl,"T9",3);

    char* string="KOKOOOOT";


    tmp_htab_insert(tabulka,tString,string,"T10",4);

    string ="PICAAAAAAAA";

    tmp_htab_insert(tabulka,tString,string,"T151",0);
    tmp_htab_insert(tabulka,tString,"DSAA_DSAD_DSA","T48",1);
    tmp_htab_insert(tabulka,tString,"S__DSD___SSSS_","T64",2);
    tmp_htab_insert(tabulka,tString,"LDS_THC","T65",3);

    Print_table(tabulka);

    stack_frame_pop(Stack);
    stack_frame_pop(Stack);
    stack_frame_pop(Stack);
    stack_frame_pop(Stack);
    clearAll();


    return 0;
}*/
