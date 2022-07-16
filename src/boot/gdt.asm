
gdt_start:

gdt_null:        dd 0,0  ; entry 0 is always unused
gdt_kernel_code:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31    
gdt_kernel_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0 
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

KERNEL_CODE_SEGMENT equ gdt_kernel_code - gdt_start
KERNEL_DATA_SEGMENT equ gdt_kernel_data - gdt_start