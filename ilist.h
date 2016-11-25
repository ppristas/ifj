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
    I_MOD,
    I_CALL,
    I_RET,
    I_NOT,
    I_LESS,



} eInstrType;

//Trojadresny kod
typedef struct
{
    eInstrType instrType;  // typ instrukcie
    void *op1;        // operator 1
    void *op2;        // operator 2
    void *dest;       // operator 3
}   instr;

typedef struct item{
    instr *instrPtr;
    struct item *nextItem;
    struct item *prevItem;
}   item;

typedef struct {
    item *first;
    item *active;
    item *last;
}   ilist;


extern ilist instrList;

void generateInstruction(eInstrType type, void *op1, void *op2, void *dest);
void listInit(ilist *L);
void first(ilist *L);
void succ(ilist *L);
void insertInstruction(ilist *L, instr *I);
void destroyList(ilist *L);
item* getLast(ilist *L);
instr *getInstruction(ilist *L);
void setInstruction(ilist *L, item* instruction);
#endif // ILIST_H_INCLUDED
