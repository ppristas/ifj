#ifndef CLEANER_H
#define CLEANER_H


typedef struct tAdr{
	void *data;
	int size;
	struct tAdr *rPtr;
	struct tAdr *lPtr;
} *tAdrPtr;


typedef struct tMemory{
    tAdrPtr First;
	tAdrPtr Last;
} *tMemoryPtr;	


extern tMemoryPtr Mem;
void *mymalloc( unsigned int size);
void *myrealloc( void *adress, unsigned int size);
void clearAll();
void initCleaner();
void myfree(void *uk);

#endif
