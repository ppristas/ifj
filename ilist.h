#ifndef ILIST_H_INCLUDED
#define ILIST_H_INCLUDED


//kod instrukcie
typedef enum
{
    I_ASSIGN,
    I_ADD,
    I_SUB,
    I_DIV,
    I_MUL,
    I_CALL,
    I_RET,
    I_NOT,
    I_NOTEQ,
    I_LESS,
    I_LESSEQ,
    I_BIGGER,
    I_BIGGEREQ,
    I_EQ,
    I_LABEL,
    I_JMP,
    I_IFJMP,



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


//Globálna páska na inštrukcie súvisiace s globálnymi premennými
extern ilist globalList;
//Globálna páska na inštrukcie v rámci Mainu, aby bola prístupnejšia nie len z TS
extern ilist mainList;

//Inicializácia globálnej pásky
void listInit(ilist *L);
//Nagenerovanie a vloženie Inštrukcie do inštrukčnej pásky
void generateLastInstruction(eInstrType type, void *op1, void *op2, void *dest, ilist *L);
//Nagenerovanie a vloženie prvku za aktívny prvok, toto sa bude využívať pri podmienkach
void generatePostInstruction(eInstrType type, void *op1, void *op2, void *dest, ilist *L);

//Nastavenie aktívneho prvku na prvý prvok
void actFirst(ilist *L);
//Nastavenie aktívneho prvku na posledný prvok, spolu s prevInstuction sa bude musieť použiť na podmienky
void actLast(ilist *L);
//Komentár o jedno vyššie
void prevInstruction(ilist *L);
//Iterácia aktívneho prvku o jedno
void succ(ilist *L);
//Získa sa pointer na posledný prvok
item* getLast(ilist *L);

//Na tieto 2 inštrukcie jebte
void insertPostIntruction(ilist *L, instr *I);
void insertLastInstruction(ilist *L, instr *I);
//Zničí sa list
void destroyList(ilist *L);
//Získa sa inštrukcia, jebte na to
instr *getInstruction(ilist *L);
//Inštrukcia na skoky
void setInstruction(ilist *L, item* instruction);
#endif // ILIST_H_INCLUDED
