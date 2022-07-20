;----------------------
dochar:
    call cprint              ; print one character
sprint:
   cld

    mov ax, [si]          ; string char to AL
    lea si, [si+1]
    cmp al, 0
    jne dochar               ; else, we're done
    add byte [ypos], 1       ; down one row
    mov byte [xpos], 0       ; back to left
    ret
 
cprint:
    mov ah, 0x03             ; attrib = white on black
    mov cx, ax             ; save char/attribute
    movzx ax, byte [ypos]
    mov dx, 160             ; 2 bytes (char/attrib)
    mul dx                  ; for 80 columns
    movzx bx, byte [xpos]
    shl bx, 1               ; times 2 to skip attrib
 
    mov di, 0xb8000         ; start of video memory
    add di, ax             ; add y offset
    add di, bx             ; add x offset
 
    mov ax, cx             ; restore char/attribute
    mov word [ds:di], ax
    add byte [xpos], 1       ; advance to right
 
    ret
 
;------------------------------------
 
printreg16:
    mov di, outstr16
    mov ax, [reg16]
    mov si, hexstr
    mov cx, 8               ; eight nibbles
 
hexloop:
    rol ax, 4               ; leftmost will
    mov bx, ax             ; become rightmost
    and bx, 0x0f ;
    mov bl, [si + bx]      ; index into hexstr
    mov [di], bl
    inc di
    dec cx
    jnz hexloop
 
    mov si, outstr16
    call sprint
 
    ret
 
;------------------------------------
 
xpos db 0
ypos db 0
hexstr db '0123456789ABCDEF'
outstr16 db '00000000', 0    ; register value
reg16 dd 0                   ; pass values to printreg16
 
;------------------------------------