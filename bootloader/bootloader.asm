; org 0x7c00

bits 16

start: 
    mov bx, 0xCC
    call MovCursor
    mov esi, msg
    call Print
    jmp boot

MovCursor:
    push ebp
    mov ebp, esp
    push ebx
    push eax
    push edx

    xor eax, eax
    mov ah, 0x2
    mov dh, bh
    mov dl, bl
    xor ebx, ebx
    int 0x10

    pop edx
    pop eax
    pop ebx
    pop ebp
    ret

PutChar:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx

    and eax, 0xFF
    mov ah, 0x09
    and ebx, 0x0F
    int 0x10

    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret

Print:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx
    push edx

    xor edx, edx
    PrintLoop:
        push ebx
        mov ebx, edx
        mov al, [ds:si + bx]
        test al, al

        jz PrintEnd
        xor eax, eax
        mov ah, 0x09
        mov al, [ds:si + bx]
        xor ebx, ebx
        mov bl, 0x03
        xor ecx, ecx
        mov cx, 1
        int 0x10
        pop ebx

        inc ebx
        call MovCursor

        inc edx
        jmp PrintLoop

    PrintEnd:
        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp
        ret


msg db "Welcome to Dolph World!", 0x0, 0ah, 0dh, 0h

boot:
    cli
    cld

    mov ax, 0x50
    mov es, ax
    xor bx, bx

    mov al, 2
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0

    mov ah, 0x02
    int 0x13
    jmp 0x50:0x0

    hlt

times 510 - ($ - $$) db 0

dw 0xaa55