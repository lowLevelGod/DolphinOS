[org 0x7c00]

start: 
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x9c00
    mov si, msg_real
    call sprint
    call switch_to_pm
    jmp $


%include "real_mode_print_hex.asm"
%include "gdt.asm"
%include "32bit-switch.asm"
%include "keyboard_handler.asm"
%include "print_string_pm.asm"

[bits 32]
boot_pm:
    mov ebx, msg_pm
    call print_string_pm
    jmp $


msg_real db "Welcome to DolphinOS! <-- Real Mode -->", 0x0
msg_pm db "Welcome to DolphinOS! <-- Protected Mode -->", 0x0

times 510 - ($ - $$) db 0x0
dw 0xaa55
