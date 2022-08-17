
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
    ; xor bx, bx
    ; xor si, si
    ; xor cx, cx
    ; xor dx, dx

    ; mov si, msg
    ; call sprint
    pusha
    call do_e820
    xchg bx, bx
    popa
    ; xchg bx, bx
    call switch_to_pm
    jmp $

load_kernel:
  xor ax, ax
  xor cx, cx
  mov ax, 25
  mov cx, 32
  ; xchg bx, bx
  call disk_load_lba
  ; xchg bx, bx
  ret

%include "lbatochs.asm"
[bits 32]

; align 4096 HARDCODED PAY ATTENTION
; times 5120 - ($ - $$) db 0x0
times 372 db 0x0

boot_page_directory:
  times 4096 db 0x0
boot_page_table1:
	times 4096 db 0x0

enable_higher_kernel_pm:

  call boot_pm

boot_pm:
  ; xchg bx, bx
	mov edi, boot_page_table1
	xor esi, esi
	mov ecx, 1024 ; Map 1024 pages

	; Map physical address as "present, writable". Note that this maps
	; .text and .rodata as writable. Mind security and map them as non-writable.
  page_table_loop:
    mov edx, esi
    or edx, 0x003
    mov dword [edi], edx

    ; Size of page is 4096 bytes.
    add esi, 4096
    ; Size of entries in boot_page_table1 is 4 bytes.
    add edi, 4
    ; Loop to the next entry if we haven't finished.
    loop page_table_loop


	; Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
  push ebx
  push eax
  xor eax, eax
  xor ebx, ebx

  mov ebx, boot_page_directory
  mov eax, boot_page_table1
  or eax, 0x003

  mov dword [ebx], eax
  add ebx, 3072 ; 768(entry) * 4
  mov dword [ebx], eax


  pop eax
  pop ebx
	; Set cr3 to the address of the boot_page_directory.
  ; xchg bx, bx
	mov ecx, boot_page_directory 
	mov cr3, ecx
  ; xchg bx, bx
	; Enable paging and the write-protect bit.
	mov ecx, cr0
	or ecx, 0x80010000
	mov cr0, ecx
  ; xchg bx, bx

	; Jump to higher half with an absolute jump. 
	lea ecx, [paging_enabled_pm]
	jmp ecx

paging_enabled_pm:
  ; xchg bx, bx
	; At this point, paging is fully set up and enabled.

	; Unmap the identity mapping as it is now unnecessary. (I DID NOT LINK THE BOOTLOADER WITH 0xC0000000, SO I WON'T DISABLE IT RN)
	; mov dword [boot_page_directory], 0x0 

	; Reload crc3 to force a TLB flush so the changes to take effect.
	mov ecx, cr3
	mov cr3, ecx

	; Set up the stack.
	mov esp, 0xC0090000

	; Enter the high-level kernel.
  xchg bx, bx
  xor ecx, ecx
  mov ecx, 0xC0010000 ; OK SO I NEED TO ADD 0X10000. HAVE TO FIX LATER
	call ecx

	; Infinite loop if the system has nothing more to do.
	cli
  hlt
	jmp $

%include "gdt.asm"
%include "32bit-switch.asm"
; %include "print_string_pm.asm"
%include "e820memory_map.asm"

msg_real db "Welcome to DolphinOS! <-- Real Mode -->", 0x0

BOOT_DRIVE db 0
KERNEL_OFFSET equ 0x1000
; KERNEL_OFFSET equ 0x0100


times 12800 - ($ - $$) db 0x0 ; 25 sectors (25 * 512 bytes)