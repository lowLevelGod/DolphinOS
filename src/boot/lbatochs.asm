[bits 16]

; maxCylinder dw 80
maxHead dw 2
maxSector dw 18
cylinder dw 0
head db 0
sector db 0
product db 0

; params ax -> lba
lba_to_chs:
    pusha
    xor         dx, dx
    mov         bx, word [maxSector]
    div         bx
    inc         dx
    mov         byte [sector], dl
    xor         dx, dx
    mov         bx, word [maxHead]
    div         bx
    mov         byte [cylinder], al
    mov         byte [head], dl
    popa
    ret	

; params ax start sector/ cx number of sectors
disk_load_lba:
    xor bx, bx
    mov bx, KERNEL_OFFSET
    mov es, bx
    xor bx, bx
    ; xchg bx, bx
    retry:
        mov di, 5
    sectorloop:
    ; xchg bx, bx
        call lba_to_chs
        pusha
        mov ah, 0x02
        mov al, 0x1
        mov ch, byte [cylinder]
        mov cl, byte [sector]
        mov dh, byte [head]
        mov dl, byte [BOOT_DRIVE]
        int 0x13
        popa
        jnc success
        sub di, 1
        jnz sectorloop
        ; xchg bx, bx
        jmp $
    success:
        mov bx, es
        add bx, 32
        mov es, bx
        xor bx, bx
        ; xchg bx, bx
        inc ax
        loop retry
        ret




