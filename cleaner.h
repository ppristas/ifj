




typedef struct tAdr{
	void *data;
	int size;
	struct tAdr *nextPtr;
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
