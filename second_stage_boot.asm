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
