#include "../../headers/kernel/utils.h"
#include "../../headers/libc/string.h"

void int_to_ascii(int x, char* s)
{
    size_t len = strlen(s);
    int c = x;
    int nr_digits = 0;
    while (c)
    {
        ++nr_digits;
        c /= 10;
    } 

    if ((size_t)nr_digits < len)
    {
        s[nr_digits] = '\0';
        len = nr_digits;
    }
    
    for (int i = len - 1; i >= 0; --i)
    {
        s[i] = x % 10 + '0';
        x /= 10;
    }
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}