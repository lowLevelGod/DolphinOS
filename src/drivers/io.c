#include "../../headers/kernel/utils.h"
#include "../../headers/drivers/io.h"
#include "../../headers/libc/string.h"

/* Hardware text mode color constants. */

void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}
 
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;
const size_t VGA_ADDRESS = 0xC03FF000;

const char shell_start_text[5] = "DOS>";
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void set_terminal_color(enum vga_color fg, enum vga_color bg)
{
	terminal_color = vga_entry_color(fg, bg);
}

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) VGA_ADDRESS;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

	terminal_writestring(shell_start_text);
}
 
void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_set_cursor (int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_removechar()
{
	terminal_putchar(0x08);
}

void terminal_putchar(char c) 
{

	if (terminal_row >= VGA_HEIGHT) {
		terminal_buffer = (uint16_t*) VGA_ADDRESS;
        int i;
        for (size_t y = 1; y < VGA_HEIGHT; y++) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				const size_t indexprev = (y - 1) * VGA_WIDTH + x;
				const size_t indexcurrent = y * VGA_WIDTH + x;
				terminal_buffer[indexprev] = terminal_buffer[indexcurrent];
		}
	}

        /* Blank last line */
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
		--terminal_row;
    }
	if (!terminal_check_specialchar(c))
	{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			++terminal_row;
		}
	}
	terminal_set_cursor(terminal_column, terminal_row);
}
 
char terminal_check_specialchar(char c)
{
	switch(c)
	{
		case '\n':
			terminal_column = 0;
			++terminal_row;
			terminal_writestring(shell_start_text);
			break;
		case 0x08:
			if (terminal_column > 0)
			{
				terminal_buffer[terminal_row * VGA_WIDTH + terminal_column - 1] = vga_entry(' ', terminal_color);
				--terminal_column;
			}
			break;
		default:
			return 0x0;
			break;
	}

	return 0x1;
}

void terminal_writestring(const char*);

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
	{
		terminal_putchar(data[i]);
	}
}
 
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
	// terminal_set_cursor(terminal_column, terminal_row);
}
