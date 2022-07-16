
bootloader: 
	nasm -f bin bootloader.asm -o bootloader.bin
interrupt:
	nasm interrupt.asm -f elf -o interrupt.o
qemu: disk_img
	qemu-system-i386 disk.img
disk_img:
	dd if=bootloader.bin of=disk.img
kernel:
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
idt:
	i686-elf-gcc -c idt.c -o idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
isr:
	i686-elf-gcc -c isr.c -o isr.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
io:
	i686-elf-gcc -c io.c -o io.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
utils:
	i686-elf-gcc -c utils.c -o utils.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
kernel_entry:
	nasm kernel_entry.asm -f elf -o kernel_entry.o
link: kernel kernel_entry interrupt idt isr utils io
	i686-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o utils.o io.o idt.o isr.o kernel.o interrupt.o --oformat binary
os-image: bootloader link
	cat bootloader.bin kernel.bin > os-image.bin
