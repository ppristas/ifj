#ifndef ILIST_H_INCLUDED
#define ILIST_H_INCLUDED


//kod instrukcie
typedef enum
{
    I_ASSIGN, //(OP,DEST, -)
    I_ADD, //(OP1,OP2, dest)
    I_SUB, //(OP1,OP2, dest)
    I_DIV, //(OP1, OP2, dest)
    I_MUL, //(OP1, OP2, dest)
    I_NOTEQ, //(OP1,OP2,dest)
    I_LESS, //(OP1,OP2,dest)
    I_LESSEQ, //(OP1,OP2,dest)
    I_BIGGER, //(OP1,OP2,dest)
    I_BIGGEREQ, //(OP1,OP2,dest)
    I_EQ, //(OP1,OP2,dest)
    I_LABEL, //(-,-,-)
    I_JMP, //(OP1, -, -) v tomto prípade je OP1 adresa inštrukcie na ktoru sa bude skakat
    I_IFJMP, //(OP1, OP2, -) OP1 adresa, OP2 boolean vysledok logickej operácie
    I_PRINT, //(OP1, -, -) OP1 je adresa prvku ktory sa bude tlacit
    I_READI, //(-, -, dest) nacita INT zo vstupu
    I_READD, //(-, -, dest) Nacita Double zo vstupu
    I_READS, //(-, -, dest) Nacita String zo vstupu
    I_STRLEN,   // (op1, -, dest) Dlzka retazca (vystup int)
    I_STRCMP, // (op1, op2, dest) Porovnanie retazcov (vystup int)
    I_STRSORT, // (op1, -, dest) Sort stringu, vystup v Dest
    I_STRSUB,
    I_STRFIND,
    I_FRAME, // (op1, -, -) OP1 je adresa funkcie vola sa prve pred I_PUSHPARAM
    I_CALL, // (op1, -, dest) OP1 je adresa funkcie vola sa posledne po I_PUSHPARAM, dest je miesto priradenia navratovej hodnoty (ak nie je tak NULL)
    I_PUSHPARAM, //(op1, -, -) OP1 je argument, ktory sa pushuje
    I_RET, // (op1, -, -) hodnota vysledku returnu, priradi sa do adresy, ktora bola urcena pri CALLE funkcie




} eInstrType;

//Trojadresny kod
typedef struct
{
    eInstrType instrType;  // typ instrukcie
    void *op1;        // operator 1
    void *op2;        // operator 2
    void *dest;       // operator 3
}   instr;

//Prvok inštrukčnej pásky
typedef struct item{
    instr *instrPtr;
    struct item *nextItem;
    struct item *prevItem;
}   item;

//Inštrukčná páska
typedef struct {
    item *first;
    item *active;
    item *last;
}   ilist;


typedef struct stackItem{
    item* instrPtr;
    ilist* list;
    struct stackItem* nextItem;
}stackItem;

typedef struct instrStack{
    stackItem* top;
    int pocet;
}instrStack;


//Globálna páska na inštrukcie súvisiace s globálnymi premennými
extern ilist globalList;
//Globálna páska na inštrukcie v rámci Mainu, aby bola prístupnejšia nie len z TS
extern ilist * currentList;

//Inicializácia globálnej pásky
void listInit(ilist *L);

item* generateItem (eInstrType type, void *op1, void *op2, void *dest);
void insertItem(item * I, ilist *L);

//Nagenerovanie a vloženie Inštrukcie do inštrukčnej pásky
void generateLastInstruction(eInstrType type, void *op1, void *op2, void *dest, ilist *L);

//Nastavenie aktívneho prvku na prvý prvok
void actFirst(ilist *L);
//Nastavenie aktívneho prvku na posledný prvok
void actLast(ilist *L);
//Iterácia aktívneho prvku o jedno
void succ(ilist *L);
//Ziska sa pointer na posledný prvok
item* getLast(ilist *L);


//void insertPostIntruction(ilist *L, instr *I);
void insertLastInstruction(ilist *L, instr *I);
//Zničí sa list
void destroyList(ilist *L);
//Získa sa inštrukcia, jebte na to
instr *getInstruction(ilist *L);
//Inštrukcia na skoky
void setInstruction(ilist *L, item* instruction);

//Inicializuje stack na odkladanie pasok
void stackInitList(instrStack *stack);
//Pushne aktualnu pasku a instrukciu na stack
void stackPushList(instrStack *stack, item* instrPtr, ilist *L);
//Vypopne poslednú instrukciu a pasku
void stackPopList(instrStack *stack);
//Ziska sa adresa najvyssieho listu
ilist * stackTopList(instrStack *stack);
//Ziska sa adresa najvyssej instrukcie
item * stackTopInstruction(instrStack *stack);
//Zrusi sa list
void stackDestroyList(instrStack * stack);

#endif // ILIST_H_INCLUDED