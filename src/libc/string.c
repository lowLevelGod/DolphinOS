#include "../../headers/libc/string.h"
#include "../../headers/drivers/io.h"
#include "../../headers/kernel/utils.h"
#include <stdarg.h>

int memcmp(const void *a, const void *b, size_t n)
{
	const unsigned char *a1 = (const unsigned char *)a;
	const unsigned char *b1 = (const unsigned char *)b;

	for (size_t i = 0; i < n; ++i)
		if (a1[i] < b1[i])
			return -1;
		else if (a1[i] > b1[i])
			return 1;

	return 0;
}

void *memmove(void *dstptr, const void *srcptr, size_t size)
{
	unsigned char *dst = (unsigned char *)dstptr;
	const unsigned char *src = (const unsigned char *)srcptr;
	if (dst < src)
	{
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	}
	else
	{
		for (size_t i = size; i != 0; i--)
			dst[i - 1] = src[i - 1];
	}
	return dstptr;
}

void *memset(void *bufptr, int value, size_t size)
{
	unsigned char *buf = (unsigned char *)bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char)value;
	return bufptr;
}

size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void *memcpy(void *__restrict dst, const void *__restrict src, size_t nbytes)
{
	size_t i;
	unsigned char *dest = (unsigned char *)dst;
	const unsigned char *source = (const unsigned char *)src;
	for (i = 0; i < nbytes; i++)
	{
		*(dest + i) = *(source + i);
	}

	return dst;
}

void reverse(char *s)
{
	size_t len = strlen(s);
	for (size_t i = 0; i < len / 2; ++i)
	{
		s[i] = s[i] ^ s[len - i - 1];
		s[len - i - 1] = s[len - i - 1] ^ s[i];
		s[i] = s[i] ^ s[len - i - 1];
	}
}

void kprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
 
    while (*format != '\0') {
		if (*format == '%')
		{
			++format;
			switch (*format)
			{
				case 'c':
					int c = va_arg(args, int);
					terminal_putchar(c);
					break;
				case 'd':
					int d = va_arg(args, unsigned int);
					char tmp[100];
					int_to_ascii(d, tmp);
					terminal_writestring(tmp);
					break;
				case 's':
					const char* s = va_arg(args, char*);
					terminal_writestring(s);
					break;
			}
		}else if (*format == '\\')
		{
			++format;
			switch (*format)
			{
				case 'n':
					terminal_putchar('\n');
					break;
			}
		}else
		{
			terminal_putchar(*format);
		}
        ++format;
    }
 
    va_end(args);
}
