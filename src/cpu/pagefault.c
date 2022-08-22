#include "../../headers/cpu/pagefault.h"
#include "../../headers/drivers/io.h"
#include "../../headers/kernel/utils.h"
#include "../../headers/cpu/isr.h"
#include "../../headers/kernel/memory_map.h"

static void pageFaultCallback(registers_t regs)
{
    // kprintf("Page fault at address: %d\n", regs.cr2);
    uint32_t err = regs.err_code & 0x7;

    switch(err)
    {
        case 0:
            // kprintf("Supervisory process tried to read a non-present page entry\n");
            mapPage(regs.cr2);
            break;
        
        case 1:
            // kprintf("SSupervisory process tried to read a page and caused a protection fault\n");
            break;
            
        case 2:
            // kprintf("Supervisory process tried to write to a non-present page entry\n");
            mapPage(regs.cr2);
            break;
            
        case 3:
            // kprintf("Supervisory process tried to write a page and caused a protection fault\n");
            break;
            
        case 4:
            // kprintf("User process tried to read a non-present page entry\n");
            break;
            
        case 5:
            kprintf("User process tried to read a page and caused a protection fault\n");
            break;
            
        case 6:
            kprintf("User process tried to write to a non-present page entry\n");
            break;
            
        case 7:
            kprintf("User process tried to write a page and caused a protection fault\n");
            break;
    }
}

void init_pageFault() {
   register_interrupt_handler(14, pageFaultCallback); 
}