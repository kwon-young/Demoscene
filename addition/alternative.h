
//Copyright BDE NE666 : C'est Merveilleux !(TM)

#ifndef ___RSTRING__OPERATION
#define ___RSTRING__OPERATION

#include <stdlib.h>
#include <string.h>

typedef char* (*rstr_op) (const char*, const char*, const unsigned int);

char* binary_reversed_string_operator_add(const char* a, const char *b);
char* reversed_string_operator_add(const char* a,const char *b,const unsigned int base);
char* unreversed_string_operation(rstr_op op ,const char* a, const char *b, unsigned int base);
char* get_reversed_string(const char* str);
void reverse_string(char* str);

#endif