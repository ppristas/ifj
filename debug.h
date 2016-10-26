#include <stdio.h>
#include <stdarg.h>

#ifndef DEBUG_H
#define DEBUG_H

// funkcia pre debugovanie
// pre DEBUG ponechat #define DEBUG
// pre odstranenie debugu zakomentovat #define DEBUG

#define DEBUG

void debug(char *format,...);


#endif
