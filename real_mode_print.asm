; THIS PRINTS USING BIOS



; Move a cursor to a specific location on screen 
; BH = Y COORD
; BL = X COORD

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

; Print a character on screen at the current cursor positon
; AL = CHAR TO PRINT
; BL = TEXT COLOR
; CX = NUMBER OF TIMES CHAR IS REPEATED

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

; PRINTS STRING
; DS:SI = ZERO TERMINATED STRING

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
