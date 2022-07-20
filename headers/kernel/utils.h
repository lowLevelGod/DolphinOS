#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include <stddef.h>

void int_to_ascii(uint64_t, char*);
void append(char s[], char n);
void backspace(char s[]);
int strcmp(char s1[], char s2[]);

#endif