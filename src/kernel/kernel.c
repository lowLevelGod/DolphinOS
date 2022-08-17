#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../../headers/cpu/isr.h"
#include "../../headers/drivers/io.h"
#include "../../headers/cpu/timer.h"
#include "../../headers/cpu/pagefault.h"
#include "../../headers/drivers/keyboard.h"
#include "../../headers/kernel/utils.h"
#include "../../headers/kernel/memory_map.h"

void init()
{
    terminal_initialize();
    init_pmm();
    init_paging();
    isr_install();
    asm volatile("sti");
    init_keyboard();
    init_pageFault();
}

void main(void) 
{
    init();
    kprintf("Welcome to DolphinOS\n");
    uint32_t* ptr = (uint32_t*)0xD0000000;
    uint32_t* ptr2 = (uint32_t*)0xD0010000;
    *ptr = 1;
    *ptr = 39;
    // *ptr2 = 5;
    // uint32_t tmp = *ptr2;
    kprintf("Value %d written to address %d\n", *ptr2, ptr2); 
    kprintf("Value %d written to address %d\n", *ptr, ptr);// for some reason value is 0 every time
	// terminal_writestring("Welcome to DolphinOS\n");
	// // terminal_writestring("Damn this really works!\n");

    // // asm volatile("xchg %bx, %bx");
    // // asm volatile("int $2");
    // asm volatile("int $2");
    // // init_timer(50);
    // /* Comment out the timer IRQ handler to read
    //  * the keyboard IRQs easier */
}