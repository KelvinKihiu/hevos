#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char* str);
int strnlen(const char* str, int max);
bool is_digit(char c);
int to_numeric_digit(char c);
char* strcpy(char* dest, char* src);
int strncmp(const char* str1, const char* str2, int n);
int strnlen_terminator(const char* str, int max, char terminator);
char tolower(char s1);
int istrncmp(const char* s1, const char* s2, int n);
char* strncpy(char* dest, const char* src, int count);

#endif