[bits 16]

; maxCylinder dw 80
maxHeadFirstStage dw 2
maxSectorFirstStage dw 18
cylinderFirstStage dw 0
headFirstStage db 0
sectorFirstStage db 0
productFirstStage db 0

; params ax -> lba
lba_to_chsFirstStage:
    pusha
    xor         dx, dx
    mov         bx, word [maxSectorFirstStage]
    div         bx
    inc         dx
    mov         byte [sectorFirstStage], dl
    xor         dx, dx
    mov         bx, word [maxHeadFirstStage]
    div         bx
    mov         byte [cylinderFirstStage], al
    mov         byte [headFirstStage], dl
    popa
    ret	

; params ax start sector/ cx number of sectors
disk_load_lbaFirstStage:
    xor bx, bx
    mov es, bx
    mov bx, second_stage
    ; xchg bx, bx
    retryFirstStage:
        mov di, 5
    sectorloopFirstStage:
    ; xchg bx, bx
        call lba_to_chsFirstStage
        pusha
        mov ah, 0x02
        mov al, 0x1
        mov ch, byte [cylinderFirstStage]
        mov cl, byte [sectorFirstStage]
        mov dh, byte [headFirstStage]
        mov dl, byte [BOOT_DRIVE]
        int 0x13
        popa
        jnc successFirstStage
        sub di, 1
        jnz sectorloopFirstStage
        ; xchg bx, bx
        jmp $
    successFirstStage:
        add bx, 512
        ; xchg bx, bx
        inc ax
        loop retryFirstStage
        ret
