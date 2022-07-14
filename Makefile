
bootloader: 
	nasm -f bin bootloader.asm -o bootloader.bin
qemu: disk_img
	qemu-system-i386 disk.img
disk_img:
	dd if=bootloader.bin of=disk.img
kernel:
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra