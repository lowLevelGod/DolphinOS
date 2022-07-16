#include "../../headers/drivers/keyboard.h"
#include "../../headers/drivers/io.h"
#include "../../headers/cpu/isr.h"
#include "../../headers/kernel/utils.h"

static void keyboard_callback(registers_t regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = inb(0x60);
    char *sc_ascii;
    int_to_ascii(scancode, sc_ascii);
    terminal_writestring("Keyboard scancode: ");
    terminal_writestring(sc_ascii);
    terminal_writestring(", ");
    print_letter(scancode);
    terminal_writestring("\n");
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}

void print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            terminal_writestring("ERROR");
            break;
        case 0x1:
            terminal_writestring("ESC");
            break;
        case 0x2:
            terminal_writestring("1");
            break;
        case 0x3:
            terminal_writestring("2");
            break;
        case 0x4:
            terminal_writestring("3");
            break;
        case 0x5:
            terminal_writestring("4");
            break;
        case 0x6:
            terminal_writestring("5");
            break;
        case 0x7:
            terminal_writestring("6");
            break;
        case 0x8:
            terminal_writestring("7");
            break;
        case 0x9:
            terminal_writestring("8");
            break;
        case 0x0A:
            terminal_writestring("9");
            break;
        case 0x0B:
            terminal_writestring("0");
            break;
        case 0x0C:
            terminal_writestring("-");
            break;
        case 0x0D:
            terminal_writestring("+");
            break;
        case 0x0E:
            terminal_writestring("Backspace");
            break;
        case 0x0F:
            terminal_writestring("Tab");
            break;
        case 0x10:
            terminal_writestring("Q");
            break;
        case 0x11:
            terminal_writestring("W");
            break;
        case 0x12:
            terminal_writestring("E");
            break;
        case 0x13:
            terminal_writestring("R");
            break;
        case 0x14:
            terminal_writestring("T");
            break;
        case 0x15:
            terminal_writestring("Y");
            break;
        case 0x16:
            terminal_writestring("U");
            break;
        case 0x17:
            terminal_writestring("I");
            break;
        case 0x18:
            terminal_writestring("O");
            break;
        case 0x19:
            terminal_writestring("P");
            break;
		case 0x1A:
			terminal_writestring("[");
			break;
		case 0x1B:
			terminal_writestring("]");
			break;
		case 0x1C:
			terminal_writestring("ENTER");
			break;
		case 0x1D:
			terminal_writestring("LCtrl");
			break;
		case 0x1E:
			terminal_writestring("A");
			break;
		case 0x1F:
			terminal_writestring("S");
			break;
        case 0x20:
            terminal_writestring("D");
            break;
        case 0x21:
            terminal_writestring("F");
            break;
        case 0x22:
            terminal_writestring("G");
            break;
        case 0x23:
            terminal_writestring("H");
            break;
        case 0x24:
            terminal_writestring("J");
            break;
        case 0x25:
            terminal_writestring("K");
            break;
        case 0x26:
            terminal_writestring("L");
            break;
        case 0x27:
            terminal_writestring(";");
            break;
        case 0x28:
            terminal_writestring("'");
            break;
        case 0x29:
            terminal_writestring("`");
            break;
		case 0x2A:
			terminal_writestring("LShift");
			break;
		case 0x2B:
			terminal_writestring("\\");
			break;
		case 0x2C:
			terminal_writestring("Z");
			break;
		case 0x2D:
			terminal_writestring("X");
			break;
		case 0x2E:
			terminal_writestring("C");
			break;
		case 0x2F:
			terminal_writestring("V");
			break;
        case 0x30:
            terminal_writestring("B");
            break;
        case 0x31:
            terminal_writestring("N");
            break;
        case 0x32:
            terminal_writestring("M");
            break;
        case 0x33:
            terminal_writestring(",");
            break;
        case 0x34:
            terminal_writestring(".");
            break;
        case 0x35:
            terminal_writestring("/");
            break;
        case 0x36:
            terminal_writestring("Rshift");
            break;
        case 0x37:
            terminal_writestring("Keypad *");
            break;
        case 0x38:
            terminal_writestring("LAlt");
            break;
        case 0x39:
            terminal_writestring("Spc");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                terminal_writestring("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                terminal_writestring("key up ");
                print_letter(scancode - 0x80);
            } else terminal_writestring("Unknown key up");
            break;
    }
}