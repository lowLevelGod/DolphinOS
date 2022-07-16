#include "../../headers/drivers/keyboard.h"
#include "../../headers/drivers/io.h"
#include "../../headers/cpu/isr.h"
#include "../../headers/kernel/utils.h"

#define terminal_buffer_size 256
#define keyboard_mapping_size 256
#define ENTER 0x1C
#define BACKSPACE 0x0E

const char keyboard_mapping[keyboard_mapping_size] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' ', '\0'};


char terminal_input_buffer[terminal_buffer_size];

void terminal_parse_command()
{
    if (!strcmp(terminal_input_buffer, "END"))
    {
        terminal_writestring("Thank you for using DolphinOS\n");
        asm volatile("hlt");
    }
}

void terminal_buffer_flush()
{
    terminal_putchar('\n');
    terminal_parse_command();
    terminal_input_buffer[0] = '\0';
    //TO DO PARSE COMMAND
}

void terminal_buffer_insert(char c)
{
    if (strlen(terminal_input_buffer) < terminal_buffer_size - 1)
    {
        append(terminal_input_buffer, c);
        char tmp[2] = {c, '\0'};
        terminal_writestring(tmp);
    }
}

void terminal_buffer_delete()
{
    if (strlen(terminal_input_buffer) > 0)
    {
        backspace(terminal_input_buffer);
        terminal_removechar();
    }

}


static void keyboard_callback(registers_t regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = inb(0x60);
    switch (scancode) {
        case 0x0E:
            // terminal_writestring("Backspace");
            terminal_buffer_delete();
            break;
        case 0x0F:
            // terminal_writestring("Tab");
            break;
        case 0x0:
            // terminal_writestring("ERROR");
            break;
        case 0x1:
            // terminal_writestring("ESC");
            break;
        case 0x1C:
			// terminal_writestring("ENTER");
            terminal_buffer_flush();
			break;
		case 0x1D:
			// terminal_writestring("LCtrl");
			break;
        case 0x2A:
			// terminal_writestring("LShift");
			break;
        case 0x36:
            // terminal_writestring("Rshift");
            break;
        case 0x37:
            // terminal_writestring("Keypad *");
            break;
        case 0x38:
            // terminal_writestring("LAlt");
            break;
        default:
            terminal_buffer_insert(keyboard_mapping[scancode]);
            break;
    }
}

void init_keyboard() {
   terminal_input_buffer[0] = '\0';
   register_interrupt_handler(IRQ1, keyboard_callback); 
}
