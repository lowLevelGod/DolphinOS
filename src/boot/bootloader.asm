[org 0x7c00]

entry_point: 
    mov [BOOT_DRIVE], dl
    xor ax, ax
    xor cx, cx
    mov ds, ax
    mov ax, 0xfff
    mov ss, ax
    mov sp, 0x0
    xor ax, ax
    ; xchg bx, bx
    ; mov bx, msg_pm
    ; call print
    ; jmp $
    ; call print_nl
    ; call load_kernel
    mov ax, 0x01
    mov cx, 25
    call disk_load_lbaFirstStage
    ; xchg bx, bx
    call second_stage
    jmp $


; %include "keyboard_handler.asm"
%include "boot_disk.asm"
; %include "boot_print.asm"

msg_pm db "Welcome to DolphinOS! <-- Protected Mode -->", 0x0


times 510 - ($ - $$) db 0x0
dw 0xaa55

%include "second_stage_boot.asm"