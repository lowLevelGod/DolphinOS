#include "../../headers/kernel/utils.h"
#include "../../headers/libc/string.h"
#include "../../headers/drivers/io.h"
#define NO_BASE 16


void int_to_ascii(uint32_t x, char* s)
{
    size_t len = 0;
    uint32_t c = x;
    if (c == 0)
        s[len++] = '0';
    while (c)
    {
        char result;
        switch (c % NO_BASE)
        {
            case 10:
                result = 'A';
                break;
            case 11:
                result = 'B';
                break;
            case 12:
                result = 'C';
                break;
            case 13:
                result = 'D';
                break;
            case 14:
                result = 'E';
                break;
            case 15:
                result = 'F';
                break;
            default:
                result = c % NO_BASE + '0';
                break;
        }
        s[len++] = result;
        c /= NO_BASE;
    } 
    s[len++] = 'x';
    s[len++] = '0';
    s[len] = '\0';
    reverse(s);
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