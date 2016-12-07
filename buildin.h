#ifndef BUILDIN_H
#define BUILDIN_H

#include "ial.h"

//TEST SMART-GIT 

char* sort(const char *str);
char* substring(const char *s, const int i, const int n);
int find(char* s,char* search);

int readInt(symData * dest);
void readString(symData * dest);
int readDouble(symData * dest);

#endif
