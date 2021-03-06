
[bits 16]

second_stage:
    ; mov si, msg_real
    ; call sprint
    
    xor ax, ax
    mov ds, ax
    call load_kernel
    xor di, di
    mov es, di
    xor ax, ax
    xor bx, bx
    xor si, si
    xor cx, cx
    xor dx, dx

    ; mov si, msg
    ; call sprint
    pusha
    call do_e820
    ; xchg bx, bx
    popa
    ; xchg bx, bx
    call switch_to_pm
    jmp $

load_kernel:
  xor ax, ax
  xor cx, cx
  mov ax, 3
  mov cx, 32
  ; xchg bx, bx
  call disk_load_lba
  ; xchg bx, bx
  ret
;     mov bx, KERNEL_OFFSET
;     mov dh, 32
;     mov dl, [BOOT_DRIVE]
    
 
;     mov  [SECTORS], dh
;     mov  ch, 0x00      ;C=0
;     mov  dh, 0x00      ;H=0
;     mov  cl, 0x04      ;S=2
;   next_group:
;     mov  di, 5         ;Max 5 tries
;   again: 
;     mov  ah, 0x02      ;Read sectors
;     mov  al, [SECTORS]
;     int  0x13
;     jc   maybe_retry
;     sub  [SECTORS], al ;Remaining sectors
;     jz  ready
;     mov  cl, 0x01      ;Always sector 1
;     xor  dh, 1         ;Next head on diskette!
;     jnz  next_group
;     inc  ch            ;Next cylinder
;     jmp  next_group
;   maybe_retry:
;     mov  ah, 0x00      ;Reset diskdrive
;     int  0x13
;     dec  di
;     jnz  again
;     jmp  disk_error2
;   ready:
;     ret

;   disk_error2:
;     mov  si, DISK_ERROR_MSG 
;     call sprint
;     jmp  $

; DISK_ERROR_MSG db "Disk read error!", 0
; SECTORS_ERROR2: db "Incorrect number", 0
; SECTORS        db 0
%include "real_mode_print_hex.asm"
%include "lbatochs.asm"
[bits 32]
boot_pm:
    ; mov ebx, msg_pm
    ; call print_string_p
    ; xchg bx, bx
    call 0x10000
    jmp $

%include "gdt.asm"
%include "32bit-switch.asm"
%include "print_string_pm.asm"
%include "e820memory_map.asm"

msg_real db "Welcome to DolphinOS! <-- Real Mode -->", 0x0

BOOT_DRIVE db 0
KERNEL_OFFSET equ 0x1000
; KERNEL_OFFSET equ 0x0100


times 1536 - ($ - $$) db 0x0