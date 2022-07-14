keyhandler:
   in al, 0x60   ; get key data
   mov bl, al   ; save it
   mov byte [port60], al
 
   in al, 0x61   ; keybrd control
   mov ah, al
   or al, 0x80   ; disable bit 7
   out 0x61, al   ; send it back
   xchg ah, al   ; get original
   out 0x61, al   ; send that back
 
   mov al, 0x20   ; End of Interrupt
   out 0x20, al   ;
 
   and bl, 0x80   ; key released
   jnz done   ; don't repeat
 
   mov ax, [port60]
   mov  word [reg32], ax
   call printreg32
 
done:
   iret
 
port60   dw 0