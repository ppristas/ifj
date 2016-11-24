#ifndef SYMTAB_H
#define SYMTB_H

#define HTAB_SIZE 32



// PSEUDOTOKEN

typedef struct
{
    char type;
    int declared_type;
}Pseudotoken;


typedef struct st_local_data
{
    char* local_key;
    int local_type;

    union
    {
        int loc_i;
        double loc_dbl;
        char* loc_str;
    }local_value;

    struct st_local_data* next_local;
}T_local_data;


typedef struct st_static_var
{
    union
    {
        int i;
        double dbl;
        char *str;
    }value;
}T_Static_var;


typedef struct st_static_func
{
    int args_count; // doplnit parametre
    T_local_data* Local_Htab[HTAB_SIZE];

}T_Static_func;

typedef struct st_static_symbol
{
    char* static_key;
    char type;              //moze byt is function
    int declared_type;
    T_Static_var *static_var;
    T_Static_func *static_func;

    struct st_static_symbol* next_static;

}T_Static_symbol;


typedef struct st_class
{
        char *class_key;
        T_Static_func* inFunction;      //adresa funkcie v ktorej sa nachadza parser
        T_Static_symbol * StaticHTab[HTAB_SIZE];
        struct st_class* next_class;
}T_ClassItem;


typedef struct st_classHtab
{
    T_ClassItem* inClass;           //adresa polozky triedy v ktorej parser je
    T_ClassItem * ClassHTab[HTAB_SIZE];
}T_ClassHtab;


void ClassHtab_init(T_ClassHtab **table);
int hash_key(char *key);
void ClassHtab_define(char *key, T_ClassHtab *table); //ked parser vojde do triedy
int ClassHtab_search_insert(char * key, T_ClassHtab *table);
T_ClassItem* ClassHtab_search(char *key, T_ClassHtab *table);
void Static_Htab_init(char* class_name, T_ClassHtab *table); //ked uz parser je v triede
int Static_Htab_search_insert(char* class_name, char* static_name, T_ClassHtab *table);
void Static_Htab_insert(char *class_name, char *static_name, T_ClassHtab *table, Pseudotoken token);
T_Static_symbol *Static_Htab_search(char* class_name,char* static_name,T_ClassHtab* table);
void Static_Var_Add(char* class_name, char* static_name, T_ClassHtab* table, void* value, int type);
void Local_table_init(T_Static_symbol *static_func);
int Local_table_search_insert(char* local_var,T_ClassHtab* table);
void Local_table_insert(char* local_var, T_ClassHtab* table, Pseudotoken token);
void Local_table_add_var(char* local_var,T_ClassHtab* table,void* value,int type);
T_local_data* Local_table_search(char* local_var,T_ClassHtab* table);



#endif