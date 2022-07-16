#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../../headers/cpu/isr.h"
#include "../../headers/drivers/io.h"
#include "../../headers/cpu/timer.h"
#include "../../headers/drivers/keyboard.h"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
void main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();
 
	// terminal_writestring("Welcome to DolphinOS\n");
	// terminal_writestring("Damn this really works!\n");

	isr_install();

    asm volatile("sti");
    // init_timer(50);
    /* Comment out the timer IRQ handler to read
     * the keyboard IRQs easier */
    init_keyboard();
}