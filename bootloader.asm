[org 0x7c00]

entry_point: 
    mov [BOOT_DRIVE], dl
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x9c00
    mov si, msg_real
    call sprint
    ; jmp $
    ; call print_nl
    call load_kernel
    call switch_to_pm
    jmp $


%include "real_mode_print_hex.asm"
%include "gdt.asm"
%include "32bit-switch.asm"
; %include "keyboard_handler.asm"
%include "print_string_pm.asm"
%include "boot_disk.asm"
; %include "boot_print.asm"

[bits 16]

load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret


[bits 32]
boot_pm:
    ; mov ebx, msg_pm
    ; call print_string_pm
    call KERNEL_OFFSET
    jmp $


msg_real db "Welcome to DolphinOS! <-- Real Mode -->", 0x0
; msg_pm db "Welcome to DolphinOS! <-- Protected Mode -->", 0x0

BOOT_DRIVE db 0
KERNEL_OFFSET equ 0x1000

times 510 - ($ - $$) db 0x0
dw 0xaa55
