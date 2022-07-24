#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../../headers/cpu/isr.h"
#include "../../headers/drivers/io.h"
#include "../../headers/cpu/timer.h"
#include "../../headers/drivers/keyboard.h"
#include "../../headers/kernel/utils.h"
#include "../../headers/kernel/memory_map.h"

void main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();
 
	// terminal_writestring("Welcome to DolphinOS\n");
	// terminal_writestring("Damn this really works!\n");

	isr_install();
    // asm volatile("xchg %bx, %bx");
    // asm volatile("int $2");
    asm volatile("sti");
    // asm volatile("int $2");
    // init_timer(50);
    /* Comment out the timer IRQ handler to read
     * the keyboard IRQs easier */
    init_keyboard();
    init_bitmap();
}