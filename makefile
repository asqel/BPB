C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o} 

# Change this if your cross-compiler is somewhere else
CC = gcc -m32
LD = ld -m elf_i386
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-stack-protector -fno-pie

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case

os-image.bin:build

run: os-image.bin
	qemu-system-i386 -fda os-image.bin -serial stdio

clean:
	make -C ./kernel clean

fclean: clean
	rm -rf os-image.bin

build:
	make -C ./kernel
	nasm -f bin boot/boot_loader.asm -o os-image.bin

