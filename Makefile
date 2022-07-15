
bootloader: 
	nasm -f bin bootloader.asm -o bootloader.bin
qemu: disk_img
	qemu-system-i386 disk.img
disk_img:
	dd if=bootloader.bin of=disk.img
kernel:
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
kernel_entry:
	nasm kernel_entry.asm -f elf -o kernel_entry.o
link: kernel kernel_entry
	i686-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary
os-image: bootloader link
	cat bootloader.bin kernel.bin > os-image.bin
