#include "../../headers/cpu/timer.h"
#include "../../headers/drivers/io.h"
#include "../../headers/kernel/utils.h"
#include "../../headers/cpu/isr.h"

uint32_t tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    terminal_writestring("Tick: ");
    
    char tick_ascii[256] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    int_to_ascii(tick, tick_ascii);
    terminal_writestring(tick_ascii);
    terminal_writestring("\n");
}

void init_timer(uint32_t freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);
    /* Send the command */
    outb(0x43, 0x36); /* Command port */
    outb(0x40, low);
    outb(0x40, high);
}
