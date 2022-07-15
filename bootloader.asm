[org 0x7c00]

entry_point: 
    mov [BOOT_DRIVE], dl
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x9c00
    ; mov bx, msg_pm
    ; call print
    ; jmp $
    ; call print_nl
    ; call load_kernel
    mov bx, second_stage
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call disk_load
    call second_stage
    jmp $


; %include "keyboard_handler.asm"
%include "boot_disk.asm"
%include "boot_print.asm"

msg_pm db "Welcome to DolphinOS! <-- Protected Mode -->", 0x0


times 510 - ($ - $$) db 0x0
dw 0xaa55

%include "second_stage_boot.asm"