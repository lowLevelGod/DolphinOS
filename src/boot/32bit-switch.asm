[bits 16]
switch_to_pm:
    ; mov bx, msg_real
    ; call print
    ; jmp $
    cli
    lgdt [gdt_descriptor]
    xor ax, ax
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp dword KERNEL_CODE_SEGMENT:init_pm

[bits 32]
init_pm:
; jmp $
    mov ax, KERNEL_DATA_SEGMENT
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp
    call boot_pm