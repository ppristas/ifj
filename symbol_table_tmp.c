#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "symtab.h"
#include "cleaner.h"

T_ClassHtab * global_table;



void ClassHtab_init(T_ClassHtab **table)
{
   if ((*table=(T_ClassHtab *)mymalloc(sizeof(T_ClassHtab)*1)) == NULL )
       return;

    for (int i=0;i< HTAB_SIZE ;++i)
    {
        (*table)->ClassHTab[i]=NULL;
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
void ClassHtab_define(char* key,T_ClassHtab *table)
{
    int index;
    if ((index=ClassHtab_search_insert(key,table)) == -1)
    {
        printf("Semantic error ! Can not init Class already exists !\n");
    }
    else
    {
        T_ClassItem * new_item;
        if ((new_item=(T_ClassItem*)mymalloc(sizeof(T_ClassItem)*1)) == NULL)
            return;
        if ((new_item->class_key=(char*)mymalloc(sizeof(char)* ((int)strlen(key) + 1))) == NULL)
            return;

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
        }
    }
    else
    {
       printf("Class %s nebola najdena\n",class_name);
    }
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
void Static_Htab_define(char* class_name,char* static_name,T_ClassHtab* table,Pseudotoken token)
{
    int index;
    T_ClassItem* class_ptr;

    if (class_name == NULL)
        class_ptr=table->inClass;
    else
        class_ptr=ClassHtab_search(class_name,table);

    if (class_ptr== NULL)
    {
        printf("Trieda nebola najdena\n");
        return;
    }


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
        return;
    }
    else
    {
        T_Static_symbol* new_symbol;
        if ((new_symbol=(T_Static_symbol*)mymalloc(sizeof(T_Static_symbol)*1)) == NULL)
            return;
        if ((new_symbol->static_key=(char*)mymalloc(sizeof(char)* ((int)strlen(static_name) + 1))) == NULL)
            return;
        strcpy(new_symbol->static_key,static_name);
        new_symbol->next_static=class_ptr->StaticHTab[index];
        class_ptr->StaticHTab[index]=new_symbol;


        new_symbol->isFunction=token.type;
        new_symbol->declared_type=token.declared_type;

        // AK type je static funkcia alebo  static variable !!! TOKENY
        if (token.type == true)
        {
            if ((new_symbol->static_func=(T_Static_func*)mymalloc(sizeof(T_Static_func)*1)) == NULL)
                return;
        }
        else if (token.type == false)
        {
            if ((new_symbol->static_var=(T_Static_var*)mymalloc(sizeof(T_Static_var)*1)) == NULL )
                return;


            //inicializacia na 0 pre staticku premennu + string dat

            if (token.declared_type==T_INT)
                    new_symbol->static_var->value.i=0;
            else if(token.declared_type==T_DOUBLE)
                    new_symbol->static_var->value.dbl=0.0;
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

    if (class_ptr == NULL)
    {
        printf("Trieda nebola najdena\n");
        return NULL;
    }

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

    if ((static_tmp = Static_Htab_search(class_name,static_name,table)) == NULL || (static_tmp->isFunction == true))
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
        if (type == T_INT)
            static_tmp->static_var->value.i=(*(int*)(value));
        else if (type == T_DOUBLE)
            static_tmp->static_var->value.dbl=(*(double*)(value));
        else if (type == T_STRING)
            static_tmp->static_var->value.str=((char*)(value));
    }
}

//do druheho parametru najebat Static_Htab_search
//nastavit context classy na citanie aktualnej funkcie
void Local_table_init(T_Static_symbol* static_func)
{
    //context
    T_Static_func* curr_static_func=static_func->static_func;

    if (curr_static_func != NULL)
    {
        for (int i=0;i<HTAB_SIZE;++i)
        {
            curr_static_func->Local_Htab[i]=NULL;
        }
    }
    else
    {
        printf("Something went wrong in function pointer!\n");
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
        T_local_data* local_tmp;

        if ((local_tmp=(T_local_data*)mymalloc(sizeof(T_local_data)*1)) == NULL)
            return;

        if ((local_tmp->local_key=(char*)mymalloc(sizeof(char)*((int)strlen(local_var)+1))) == NULL)
            return;
        strcpy(local_tmp->local_key,local_var);
        local_tmp->local_type=token.declared_type;
        local_tmp->next_local=table->inClass->inFunction->Local_Htab[index];
        table->inClass->inFunction->Local_Htab[index]=local_tmp;

        //inicializacia hodnot na nuly + doplnit string
        if (token.declared_type==T_INT)
            local_tmp->local_value.loc_i=0;
        else if (token.declared_type==T_DOUBLE)
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
        if (type==T_INT)
            local_tmp->local_value.loc_i=(*(int*)(value));
        else if (type == T_DOUBLE)
            local_tmp->local_value.loc_dbl=(*(double*)(value));
        else if (type == T_STRING)
            local_tmp->local_value.loc_str=((char*)(value));
    }
}



static void Print_chain(T_ClassItem *class_item);
static void ClassHtab_print(T_ClassHtab *table);
static void Print_static_chain(T_Static_symbol *static_item);
static void Print_static_htab(T_ClassItem *class_item);
static void Print_local(T_Static_symbol* static_function);
static void Print_local_chain(T_local_data* local_data);
static Pseudotoken Pseudotoken_init(Pseudotoken* token,bool type,VAR_TYPE declared_type);

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

        if (table->ClassHTab[i] != NULL)
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

        if (tmp->local_type==T_INT)
            printf("%d) ",tmp->local_value.loc_i);
        else if (tmp->local_type==T_DOUBLE)
            printf("%f) ",tmp->local_value.loc_dbl);
        else if (tmp->local_type==T_STRING)
            printf("%s) ",tmp->local_value.loc_str);

        tmp=tmp->next_local;
    }
}

static void Print_local(T_Static_symbol* static_function)
{
    T_Static_func* tmp=static_function->static_func;

    for (int i=0; i<HTAB_SIZE; ++i )
    {
        Print_local_chain(tmp->Local_Htab[i]);

        if (tmp->Local_Htab[i] != NULL)
            printf("\n");
    }

}

static Pseudotoken Pseudotoken_init(Pseudotoken* token, bool type, VAR_TYPE declared_type)
{
    token->declared_type=declared_type;
    token->type=type;
    return (*token);
}

int main(int argc, char *argv[])
{

    initCleaner();
    // inicializacia
    ClassHtab_init(&global_table);

    Pseudotoken TOKEN;

    double dval;
    int ival;
    T_Static_symbol * tmp;
    T_Static_symbol* context;


    //pre parser
    //JAVA KOD

    /*
     * class MyClass {
     *      static int var1;
     *      static void funk1(int a,int b);
     *      static int funk2(void) {
     *          int var1;
     *          double var2=20.2;
     *
     *          var1=5;
     *          MyClass.var1=200;
     *          int var3;
     *
     *          MyClass.var2=50;
     *      }
     *      static int var2;
     *      static void funk1(int a,int b){
     *          int varX;
     *          int varY;
     *      }
     * }
     *
     *
     * class Main {
     *      static int Mvar=50;
     *      static void run(){
     *          MyClass.funk2();
     *          Mvar=80;
     *      }
     *  }
     */

    // prvy priechod ??
    ClassHtab_define("MyClass",global_table);
    Static_Htab_init("MyClass",global_table);

    Static_Htab_define("MyClass","var1",global_table,Pseudotoken_init(&TOKEN,false,T_INT));
    Static_Htab_define("MyClass","funk1",global_table,Pseudotoken_init(&TOKEN,true,T_VOID));

    if (Static_Htab_search("MyClass","funk2",global_table) == NULL)
        Static_Htab_define("MyClass","funk2",global_table,Pseudotoken_init(&TOKEN,true,T_INT));

    Static_Htab_define("MyClass","var2",global_table,Pseudotoken_init(&TOKEN,false,T_INT));

    if (Static_Htab_search("MyClass","funk1",global_table) == NULL)
        Static_Htab_define("MyClass","funk1",global_table,Pseudotoken_init(&TOKEN,true,T_INT));


    ClassHtab_define("Main",global_table);
    Static_Htab_init("Main",global_table);

    Static_Htab_define("Main","Mvar",global_table,Pseudotoken_init(&TOKEN,false,T_INT));
    Static_Htab_define("Main","run",global_table,Pseudotoken_init(&TOKEN,true,T_VOID));

    // druhy priechod ??
    // ulozena hodnota do global table - vie ze je v triede MyClass
    global_table->inClass=ClassHtab_search("MyClass",global_table);

    // Moze previest operacie Static_Htab_search aby sa zistilo ci su vsetky globalne premenne
    // deklarovane s prveho priechodu hadam

    // narazi na definiciu funkcie otvori lokalny kontext pre funkciu aby vedel v ktorej funkcii
    // sa nachadza

    context=Static_Htab_search(NULL,"funk2",global_table);
    global_table->inClass->inFunction=context->static_func;

    // Do parametrov moze ist kludne aj Static_Htab_search lebo vracia pointer na Symbol
    Local_table_init(context);
    Local_table_insert("var1",global_table,Pseudotoken_init(&TOKEN,false,T_INT));

    Local_table_insert("var2",global_table,Pseudotoken_init(&TOKEN,false,T_DOUBLE));

    dval=20.2;
    Local_table_add_var("var2",global_table,&dval,T_DOUBLE);
    ival=5;
    Local_table_add_var("var1",global_table,&ival,T_INT);
    ival=200;
    Static_Var_Add("MyClass","var1",global_table,&ival,T_INT);

    Local_table_insert("var3",global_table,Pseudotoken_init(&TOKEN,false,T_INT));
    ival=50;
    Static_Var_Add("MyClass","var2",global_table,&ival,T_INT);

    //koniec funkcie funk2
    //odchadzam s kontextu funkcie funk2

    global_table->inClass->inFunction=NULL;

    //...

    //nastavim kontext funk1
    context=Static_Htab_search(NULL,"funk1",global_table);
    global_table->inClass->inFunction=context->static_func;
    Local_table_init(context);

    Local_table_insert("varX",global_table,Pseudotoken_init(&TOKEN,false,T_INT));
    Local_table_insert("varY",global_table,Pseudotoken_init(&TOKEN,false,T_INT));

    //koniec funkcie funk1
    //odchadzam s kontextu funkcie funk1
    global_table->inClass->inFunction=NULL;
    // odchadzam s kontextu MyClass


    global_table->inClass=ClassHtab_search("Main",global_table);
    ival=50;
    Static_Var_Add("Main","Mvar",global_table,&ival,T_INT);
    // ak sme v kontexte kludne mozme pouzivat aj NULL namiesto mena classy
    // berie to cestu z inClass -> Static_Var_Add(NULL,"Mvar",global_table,&ival,T_INT);

    context=Static_Htab_search(NULL,"run",global_table);
    global_table->inClass->inFunction=context->static_func;

    // v tomto mieste este posetrit ak by bola Mvar ako lokalna tak lokalna ma vo funkci
    // prednost pred globalnou
    ival=80;
    Static_Var_Add(NULL,"Mvar",global_table,&ival,T_INT);

    //opustenie kontextu funkcie aj triedy
    global_table->inClass->inFunction=NULL;
    global_table->inClass=NULL;


    printf("VYPIS TABULKY PRE PROGRAM\n");

    ClassHtab_print(global_table);

    printf("VYPIS HODNOT\n");

    tmp=Static_Htab_search("MyClass","var1",global_table);
    printf("var1 z MyClass = %d\n",tmp->static_var->value.i);

    tmp=Static_Htab_search("MyClass","var2",global_table);
    printf("var2 z MyClass = %d\n",tmp->static_var->value.i);

    printf("funk2 z MyClass lokalne\n");
    Print_local(Static_Htab_search("MyClass","funk2",global_table));

    printf("funk1 z MyClass lokalne\n");
    Print_local(Static_Htab_search("MyClass","funk1",global_table));


    tmp=Static_Htab_search("Main","Mvar",global_table);
    printf("Mvar z Main = %d\n",tmp->static_var->value.i);

    printf("run z Main lokalne\n");
    Print_local(Static_Htab_search("Main","run",global_table));



    clearAll();

    return 0;
}
