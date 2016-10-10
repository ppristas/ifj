#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED


typedef struct {
    char* str;
    int length;
    int allocated;
}string;


int string_init(string *s);
void string_free(string *s);
void string_fill(string *s, char c);
int string_realloc(string *s);
void string_print(string *s);
int string_merge(string *s1, string *s2);
void string_delChar(string *s);
#endif // STRING_H_INCLUDED
