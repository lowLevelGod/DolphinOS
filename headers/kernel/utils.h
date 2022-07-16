#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include <stddef.h>

size_t strlen(const char*);
void int_to_ascii(int, char*);
void memory_copy(char *source, char *dest, int nbytes);

#endif