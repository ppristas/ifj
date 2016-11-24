#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symtab.h"
#include "cleaner.h"

T_ClassHtab * global_table;



void ClassHtab_init(T_ClassHtab **table)
{
    *table=(T_ClassHtab *)mymalloc(sizeof(T_ClassHtab)*1);

    for (int i=0;i< HTAB_SIZE ;++i)
    {
        (*table)->ClassHTab[i]=NULL;
         printf("Init %d  na NULL: \n",i);
    }
}



int hash_key(char *key)
{
    unsigned long hash = 5381;
    int c;

     while (c = *key++)
         hash = ((hash << 5) + hash) + c;

     return (hash % HTAB_SIZE);
}


//pri kolizi dvoch nazvov vracia -1
//inak index kde sa nachadza trieda

int ClassHtab_search_insert(char * key,T_ClassHtab * table)
{
    int index=hash_key(key);

    T_ClassItem *tmp=(table->ClassHTab[index]);

    while (tmp !=NULL)
    {
        if (strcmp(tmp->class_key,key) == 0)
            return -1;
        tmp=tmp->next_class;
    }

    return index;
}

//DEKLARACIA TRIEDY
void ClassHtab_insert(char* key,T_ClassHtab *table)
{
    int index;
    if ((index=ClassHtab_search_insert(key,table)) == -1)
    {
        printf("Semantic error ! Can not init Class already exists !\n");
    }
    else
    {
        T_ClassItem * new_item=(T_ClassItem*)mymalloc(sizeof(T_ClassItem)*1);
        new_item->class_key=(char*)mymalloc(sizeof(char)* ((int)strlen(key) + 1));
        strcpy(new_item->class_key,key);
        new_item->next_class=table->ClassHTab[index];
        table->ClassHTab[index]=new_item;
    }

}


T_ClassItem* ClassHtab_search(char *key, T_ClassHtab *table)
{
    int index=hash_key(key);

    T_ClassItem *tmp=(table->ClassHTab[index]);

    while (tmp !=NULL)
    {
        if (strcmp(tmp->class_key,key) == 0)
            return tmp;
        tmp=tmp->next_class;
    }

    return NULL;
}




void Static_Htab_init(char *class_name,T_ClassHtab *table)
{
    T_ClassItem * curr_class=ClassHtab_search(class_name,table);

    if (curr_class != NULL)
    {
        for (int i=0;i< HTAB_SIZE ;++i)
        {
            curr_class->StaticHTab[i]=NULL;
            printf("Static %d of class %s  na NULL: \n",i,curr_class->class_key);
        }
    }
    else
       printf("Class %s nebola najdena\n",class_name);
}



// class_name nemusi vobec byt kvoli tomu ze sme v konkretnej triede a tu mame ulozenu
int Static_Htab_search_insert(char* class_name,char* static_name,T_ClassHtab* table)
{
    T_ClassItem* class_ptr;
    if (class_name == NULL)
        class_ptr=table->inClass;  //priamy pristup
    else
        class_ptr=ClassHtab_search(class_name,table);

    int index=hash_key(static_name);
    T_Static_symbol* tmp=class_ptr->StaticHTab[index];

    while (tmp !=NULL)
    {
        if (strcmp(tmp->static_key,static_name) == 0)
            return -1;
        tmp=tmp->next_static;
    }

    return index;

}

// opat class name nemusi byt berieme direct
//DEKLARACIA STATIC VARIABLE alebo FUNKCIE
void Static_Htab_insert(char* class_name,char* static_name,T_ClassHtab* table,Pseudotoken token)
{
    int index;
    T_ClassItem* class_ptr;

    if (class_name == NULL)
        class_ptr=table->inClass;
    else
        class_ptr=ClassHtab_search(class_name,table);


    //DEBUG PODMIENKA----------------------------
    char* DEBUG_class_name;

    if (class_name == NULL)
        DEBUG_class_name=table->inClass->class_key;
    else
        DEBUG_class_name=class_name;
    //DEBUG PODMIENKA----------------------------


    if ((index=Static_Htab_search_insert(class_name,static_name,table))  == -1)
    {
        printf("Semantic error ! Static type %s of class %s already exists !\n",static_name,DEBUG_class_name);
    }
    else
    {
        T_Static_symbol* new_symbol=(T_Static_symbol*)mymalloc(sizeof(T_Static_symbol)*1);
        new_symbol->static_key=(char*)mymalloc(sizeof(char)* ((int)strlen(static_name) + 1));
        strcpy(new_symbol->static_key,static_name);
        new_symbol->next_static=class_ptr->StaticHTab[index];
        class_ptr->StaticHTab[index]=new_symbol;


        new_symbol->type=token.type;
        new_symbol->declared_type=token.declared_type;

        // AK type je static funkcia alebo  static variable !!! TOKENY
        if (token.type == 'f')
        {
            new_symbol->static_state.static_func=(T_Static_func*)mymalloc(sizeof(T_Static_func)*1);
        }
        else if (token.type == 'v')
        {
            new_symbol->static_state.static_var=(T_Static_var*)mymalloc(sizeof(T_Static_var)*1);


            //inicializacia na 0 pre staticku premennu + string dat

            if (token.declared_type==1)
                    new_symbol->static_state.static_var->value.i=0;
            else if(token.declared_type==2)
                    new_symbol->static_state.static_var->value.dbl=0.0;
        }
    }
}

T_Static_symbol* Static_Htab_search(char* class_name,char* static_name,T_ClassHtab* table)
{
    T_ClassItem* class_ptr;

    if (class_name == NULL)
        class_ptr=table->inClass;
    else
        class_ptr=ClassHtab_search(class_name,table);

    int index=hash_key(static_name);
    T_Static_symbol* tmp=class_ptr->StaticHTab[index];

    while (tmp != NULL)
    {
        if (strcmp(tmp->static_key,static_name) == 0)
            return tmp;
        tmp=tmp->next_static;
    }

    return NULL;
}


void Static_Var_Add(char* class_name,char* static_name,T_ClassHtab* table,void* value,int type)
{
    T_Static_symbol* static_tmp;

    if ((static_tmp = Static_Htab_search(class_name,static_name,table)) == NULL || (static_tmp->type == 'f'))
    {
        printf("Static Variable of class %s does not exist !\n",static_name);
    }
    else if (static_tmp->declared_type != type)
    {
        printf("Static Variable: Type mišmaš!\n");
    }
    else
    {
        // KONTROLA TYPU + VKLADANIE TYPU !!!
        if (type == 1)
            static_tmp->static_state.static_var->value.i=(*(int*)(value));
        else if (type == 2)
            static_tmp->static_state.static_var->value.dbl=(*(double*)(value));
        else if (type == 3)
            static_tmp->static_state.static_var->value.str=((char*)(value));
    }
}

//do druheho parametru najebat Static_Htab_search
//nastavit context classy na citanie aktualnej funkcie
void Local_table_init(T_Static_symbol* static_func)
{
    //context
    T_Static_func* curr_static_func=static_func->static_state.static_func;

    for (int i=0;i<HTAB_SIZE;++i)
    {
        curr_static_func->Local_Htab[i]=NULL;
    }
}

//musi byt nastaveny context citania
int Local_table_search_insert(char* local_var,T_ClassHtab* table)
{
    int index=hash_key(local_var);
    T_local_data* tmp_local=table->inClass->inFunction->Local_Htab[index];

    while (tmp_local != NULL)
    {
        if ((strcmp(tmp_local->local_key,local_var)) == 0)
            return -1;
        tmp_local=tmp_local->next_local;
    }
    return index;
}


//DEKLARACIA LOKALNEJ PREMENNEJ
T_local_data* Local_table_search(char* local_var,T_ClassHtab* table)
{
    int index=hash_key(local_var);
    T_local_data* tmp_local=table->inClass->inFunction->Local_Htab[index];

    while (tmp_local != NULL)
    {
        if ((strcmp(tmp_local->local_key,local_var)) == 0)
            return tmp_local;
        tmp_local=tmp_local->next_local;
    }
    return NULL;
}

// type prekonvertovat na nejaky enum
void Local_table_insert(char* local_var, T_ClassHtab* table, Pseudotoken token)
{
    int index;

    if ((index=Local_table_search_insert(local_var,table)) == -1)
    {
        printf("Local variable %s already existuje!\n",local_var);
    }
    else
    {
        T_local_data* local_tmp=(T_local_data*)mymalloc(sizeof(T_local_data)*1);
        local_tmp->local_key=(char*)mymalloc(sizeof(char)*((int)strlen(local_var)+1));
        strcpy(local_tmp->local_key,local_var);
        local_tmp->local_type=token.declared_type;
        local_tmp->next_local=table->inClass->inFunction->Local_Htab[index];
        table->inClass->inFunction->Local_Htab[index]=local_tmp;

        //inicializacia hodnot na nuly + doplnit string
        if (token.declared_type==1)
            local_tmp->local_value.loc_i=0;
        else if (token.declared_type==2)
            local_tmp->local_value.loc_dbl=0.0;
    }
}

void Local_table_add_var(char* local_var,T_ClassHtab* table,void* value,int type)
{
    T_local_data* local_tmp=Local_table_search(local_var,table);

    if (local_tmp == NULL)
    {
        printf("Local variable exist!\n");
    }
    else if (local_tmp->local_type != type)
    {
        printf("Local variable: Type mišmaš!\n");
    }
    else
    {
        // TYPOVA KONTROLA !!!
        if (type==1)
            local_tmp->local_value.loc_i=(*(int*)(value));
        else if (type == 2)
            local_tmp->local_value.loc_dbl=(*(double*)(value));
        else if (type == 3)
            local_tmp->local_value.loc_str=((char*)(value));
    }
}



static void Print_chain(T_ClassItem *class_item);
static void ClassHtab_print(T_ClassHtab *table);
static void Print_static_chain(T_Static_symbol *static_item);
static void Print_static_htab(T_ClassItem *class_item);
static void Print_local(T_Static_symbol* static_function);
static void Print_local_chain(T_local_data* local_data);
static Pseudotoken Pseudotoken_init(Pseudotoken* token,char type,int declared_type);

static void Print_chain(T_ClassItem * class_item)
{
    T_ClassItem * tmp=class_item;

    while(tmp != NULL)
    {
        printf(" (%s) ",tmp->class_key);
        Print_static_htab(tmp);
        tmp=tmp->next_class;
    }
}

static void ClassHtab_print(T_ClassHtab * table)
{
    for (int i=0;i< HTAB_SIZE ;++i)
    {
        Print_chain(table->ClassHTab[i]);
        printf("\n");
    }
}

static void Print_static_chain(T_Static_symbol* static_item)
{
    T_Static_symbol* tmp=static_item;


    while(tmp != NULL)
    {
        printf(" (%s) ",tmp->static_key);
        tmp=tmp->next_static;
    }
}

static void Print_static_htab(T_ClassItem* class_item)
{
    for (int i=0; i < HTAB_SIZE; ++i)
    {
        if (class_item->StaticHTab[i] != NULL)
            printf("(%d)",i);
        Print_static_chain(class_item->StaticHTab[i]);
    }
}

static void Print_local_chain(T_local_data* local_data)
{
    T_local_data* tmp=local_data;

    while(tmp != NULL)
    {
        printf(" (%s -  ",tmp->local_key);

        if (tmp->local_type==1)
            printf("%d) ",tmp->local_value.loc_i);
        else if (tmp->local_type==2)
            printf("%f) ",tmp->local_value.loc_dbl);
        else if (tmp->local_type==3)
            printf("%s) ",tmp->local_value.loc_str);

        tmp=tmp->next_local;
    }
}

static void Print_local(T_Static_symbol* static_function)
{
    T_Static_func* tmp=static_function->static_state.static_func;

    for (int i=0; i<HTAB_SIZE; ++i )
    {
        Print_local_chain(tmp->Local_Htab[i]);
        printf("\n");
    }

}

static Pseudotoken Pseudotoken_init(Pseudotoken* token,char type,int declared_type)
{
    token->declared_type=declared_type;
    token->type=type;
    return (*token);
}

int main(int argc, char *argv[])
{
    initCleaner();
    ClassHtab_init(&global_table);

    Pseudotoken TOKEN;

    printf("Hash key %u\n", hash_key("materina pica"));
/*
    ClassHtab_insert("class1",global_table);
    ClassHtab_insert("class2",global_table);
    ClassHtab_insert("ifj16",global_table);


    Static_Htab_init("ifj16",global_table);
    Static_Htab_init("class2",global_table);
    Static_Htab_init("class1",global_table);


    global_table->inClass=ClassHtab_search("ifj16",global_table);

    Static_Htab_insert(NULL,"readString",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert(NULL,"sort",global_table,Pseudotoken_init(&TOKEN,'f',1));



    global_table->inClass=ClassHtab_search("class2",global_table);

    Static_Htab_insert(NULL,"f1",global_table,Pseudotoken_init(&TOKEN,'f',1));

    global_table->inClass=ClassHtab_search("class1",global_table);


    Static_Htab_insert(NULL,"f1",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert(NULL,"f2",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert(NULL,"f3",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert(NULL,"v3",global_table,Pseudotoken_init(&TOKEN,'v',1));
    Static_Htab_insert(NULL,"v1",global_table,Pseudotoken_init(&TOKEN,'v',1));
    Static_Htab_insert(NULL,"f584",global_table,Pseudotoken_init(&TOKEN,'f',1));

    Static_Htab_insert("ifj16","sort",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert("ifj16","readString",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert("ifj16","readStrings",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert("ifj16","substr",global_table,Pseudotoken_init(&TOKEN,'f',1));

    global_table->inClass=ClassHtab_search("class2",global_table);

    Static_Htab_insert(NULL,"f1",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert(NULL,"v1",global_table,Pseudotoken_init(&TOKEN,'v',1));
    Static_Htab_insert(NULL,"f4",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert("class1","v3",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert("class1","KOKOT",global_table,Pseudotoken_init(&TOKEN,'f',1));

    global_table->inClass=ClassHtab_search("class1",global_table);

    Static_Htab_insert(NULL,"f3",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Static_Htab_insert(NULL,"v3",global_table,Pseudotoken_init(&TOKEN,'v',1));

*/
    double dval;
    int ival;
    T_Static_symbol * tmp;

    ClassHtab_insert("class1",global_table);
    Static_Htab_init("class1",global_table);

    ClassHtab_insert("class2",global_table);
    Static_Htab_init("class2",global_table);

    ClassHtab_insert("class3",global_table);
    Static_Htab_init("class3",global_table);


    //class class2 {
    global_table->inClass=ClassHtab_search("class2",global_table);
    Static_Htab_insert(NULL,"var3",global_table,Pseudotoken_init(&TOKEN,'v',2));

    // class class1 {
    global_table->inClass=ClassHtab_search("class1",global_table);

    Static_Htab_insert(NULL,"var42",global_table,Pseudotoken_init(&TOKEN,'v',2));
    Static_Htab_insert(NULL,"X",global_table,Pseudotoken_init(&TOKEN,'v',1));


    //deklaracia funkcie a inicializacia jej tabulky symbolov
    Static_Htab_insert(NULL,"foo1",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Local_table_init(Static_Htab_search(NULL,"foo1",global_table));

    //nastavenie contextu pre citanie z foo1
    T_Static_symbol* context=Static_Htab_search(NULL,"foo1",global_table);
    global_table->inClass->inFunction=context->static_state.static_func;
    Local_table_insert("local_x",global_table,Pseudotoken_init(&TOKEN,0,1));
    ival=158;
    Local_table_add_var("local_x",global_table,&ival,2); // TYPE mismas
    Local_table_insert("local_y",global_table,Pseudotoken_init(&TOKEN,0,1));


    //TEST pre x z foo1 a x z class1

    if (Local_table_search("X",global_table) != NULL  && Static_Htab_search(NULL,"X",global_table) != NULL )
        printf("lokalny kontext pre hodnotu X\n");
    else if (Local_table_search("X",global_table) == NULL  && Static_Htab_search(NULL,"X",global_table) != NULL )
        printf("globalny kontext pre hodnotu X\n");

    Static_Htab_insert("class3","X",global_table,Pseudotoken_init(&TOKEN,'v',1));

    if (Static_Htab_search("class35","X25",global_table) == NULL)
        printf("Staticka prem  z class3 neexistuje\n");
    else
        printf("Staticka prem  z class3 existuje\n");






    //preinicializovanie statickej premennej inej existujucej funkcie
    dval=160;
    Static_Var_Add(NULL,"var42",global_table,&dval,1); //TYPE mismas
    tmp=Static_Htab_search(NULL,"var42",global_table);
    printf("%s %f\n",tmp->static_key,tmp->static_state.static_var->value.dbl);
    // POZOR  BERE TO NULL TAKZE IDE ERROR


    Local_table_insert("local_z",global_table,Pseudotoken_init(&TOKEN,0,1));
    ival=87;
    Local_table_add_var("local_z",global_table,&ival,1);

    ival=871;
    Local_table_add_var("local_y",global_table,&ival,1);



    global_table->inClass->inFunction=NULL;
    //ukoncenie contextu

    printf("End of function foo1\n");


    Static_Htab_insert(NULL,"var1",global_table,Pseudotoken_init(&TOKEN,'v',1));
    Static_Htab_insert(NULL,"var2",global_table,Pseudotoken_init(&TOKEN,'v',2));

    Static_Htab_insert(NULL,"foo2",global_table,Pseudotoken_init(&TOKEN,'f',1));
    Local_table_init(Static_Htab_search(NULL,"foo2",global_table));

    Static_Htab_insert(NULL,"var3",global_table,Pseudotoken_init(&TOKEN,'v',2));

    dval=12.2;
    Static_Var_Add(NULL,"var2",global_table,&dval,2);
    tmp=Static_Htab_search(NULL,"var2",global_table);

    printf("%s %f\n",tmp->static_key,tmp->static_state.static_var->value.dbl);



    ival=10;
    Static_Var_Add("class1","var1",global_table,&ival,1);
    tmp=Static_Htab_search(NULL,"var1",global_table);
    printf("%s %d\n",tmp->static_key,tmp->static_state.static_var->value.i);

    dval=12.345;
    Static_Var_Add("class2","var3",global_table,&dval,2);
    tmp=Static_Htab_search("class2","var3",global_table);
    printf("%s %f\n",tmp->static_key,tmp->static_state.static_var->value.dbl);

    dval=12.42;
    Static_Var_Add("class2","var3",global_table,&dval,2);
    tmp=Static_Htab_search("class2","var3",global_table);
    printf("%s %f\n",tmp->static_key,tmp->static_state.static_var->value.dbl);

    dval=16;
    Static_Var_Add("class2","var3",global_table,&dval,2);
    tmp=Static_Htab_search("class2","var3",global_table);
    printf("%s %f\n",tmp->static_key,tmp->static_state.static_var->value.dbl);




    ClassHtab_print(global_table);

    global_table->inClass=ClassHtab_search("class1",global_table);
    context=Static_Htab_search(NULL,"foo1",global_table);
    global_table->inClass->inFunction=context->static_state.static_func;

    Print_local(context);

    clearAll();





    return 0;
}
