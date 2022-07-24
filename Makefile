
C_SOURCES = $(wildcard src/boot/*.c src/cpu/*.c src/drivers/*.c src/kernel/*.c src/libc/*.c)
HEADERS = $(wildcard headers/cpu/*.h headers/drivers/*.h headers/kernel/*.h headers/libc/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o src/cpu/interrupt.o} 
OUTS = $(wildcard src/boot/*.o src/cpu/*.o src/kernel/*.o src/drivers/*.o src/libc/*.o) 
BINS = $(wildcard src/boot/*.bin src/cpu/*.bin src/kernel/*.bin src/libc/*.bin)
ELFS = $(wildcard src/boot/*.elf src/cpu/*.elf src/kernel/*.elf src/libc/*.elf)

DISK_IMG = /mnt/g/miscDolphinOS/disk.img

# Change this if your cross-compiler is somewhere else
CC = ~/opt/cross/bin/i686-elf-gcc
LD = ~/opt/cross/bin/i686-elf-ld
# GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g

all: disk_img src/kernel/kernel.elf

disk_img: src/boot/bootloader.bin src/kernel/kernel.bin
	cat $^ > $(DISK_IMG)

# src/kernel/kernel: src/boot/kernel_entry.o ${OBJ}
# 	$(LD) -m elf_i386 -o $@.elf -Ttext 0x10000 $^
# 	objcopy --only-keep-debug $@.elf kernel.sym
# 	objcopy -O binary $@.elf $@.bin

src/kernel/kernel: src/boot/kernel_entry.o ${OBJ}
	$(LD) -m elf_i386 -o $@.elf -Ttext 0x1000 $^
	objcopy --only-keep-debug $@.elf kernel.sym
	objcopy -O binary $@.elf $@.bin

src/kernel/kernel.bin: src/kernel/kernel
	objcopy -O binary src/kernel/kernel.elf $@

src/kernel/kernel.elf: src/kernel/kernel
	objcopy --only-keep-debug $@ kernel.sym

# src/kernel/kernel.bin: src/boot/kernel_entry.o ${OBJ}
# 	$(CC) -T kernel_linker.ld -o $@  -ffreestanding -O2 -nostdlib $^ -lgcc

%.o: %.c ${HEADERS}
	${CC} $(CFLAGS) -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@ -i src/boot

clean:
	rm -rf $(BINS)  $(OUTS) $(DISK_IMG) $(ELFS)