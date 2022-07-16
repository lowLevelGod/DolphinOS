#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "isr.h"
#include "io.h"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 

void shell_start_text()
{
	terminal_writestring("DoS>");
}
 
void main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();
 
	terminal_writestring("Welcome to DolphinOS\n");
	shell_start_text();
	terminal_writestring("Damn this really works!\n");

	 isr_install();
    /* Test the interrupts */
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
}