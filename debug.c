#include "debug.h"


#ifndef DEBUG
void debug(char *format,...) {}
#else
void debug(char *format,...)
{
        va_list args;

        printf("DEBUG:");
        va_start(args,format);
        vprintf(format,args);
        va_end(args);
}
#endif


/*
int main(int argc, char *argv[])
{
    char *znaky="test1";

    int test=12;

    debug("Hodnota znaky je: %s \n", znaky);

    debug("Hodnota test je: %d \n", test);

    return 0;
}
*/