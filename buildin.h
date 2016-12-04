#ifndef BUILDIN_H
#define BUILDIN_H

#include "ial.h"

//TEST SMART-GIT 

char* sort(const char *str);
int length(const char* s);
char* substring(const char *s, const int i, const int n);
int compare(const char *s1, const char *s2);
int find(char* s,char* search);

void readInt(symData * dest);
void readString(symData * dest);
void readDouble(symData * dest);

#endif
