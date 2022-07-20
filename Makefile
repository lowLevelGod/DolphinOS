
C_SOURCES = $(wildcard src/boot/*.c src/cpu/*.c src/drivers/*.c src/kernel/*.c src/libc/*.c)
HEADERS = $(wildcard headers/cpu/*.h headers/drivers/*.h headers/kernel/*.h headers/libc/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o src/cpu/interrupt.o} 
OUTS = $(wildcard src/boot/*.o src/cpu/*.o src/kernel/*.o src/drivers/*.o src/libc/*.o) 
BINS = $(wildcard src/boot/*.bin src/cpu/*.bin src/kernel/*.bin src/libc/*.bin)
ELFS = $(wildcard src/boot/*.elf src/cpu/*.elf src/kernel/*.elf src/libc/*.elf)

DISK_IMG = disk.img

# Change this if your cross-compiler is somewhere else
CC = ~/opt/cross/bin/i686-elf-gcc
LD = ~/opt/cross/bin/i686-elf-ld
# GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g

disk_img: src/boot/bootloader.bin src/kernel/kernel.bin
	cat $^ > $(DISK_IMG)

src/kernel/kernel.bin: src/boot/kernel_entry.o ${OBJ}
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@ -i src/boot

clean:
	rm -rf $(BINS)  $(OUTS) $(DISK_IMG) $(ELFS)