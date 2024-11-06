C_SOURCES = $(wildcard kernel/*.c libft/*.c kernel/*.asm, oeuf/*.c)

# Nice syntax for file extension replacement
OBJS = ${C_SOURCES:.c=.o} 
OBJS := ${OBJS:.asm=.o}

# Change this if your cross-compiler is somewhere else
CC = gcc -m32
LD = ld -m elf_i386
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-stack-protector -fno-pie -I./include -nostdlib -nostdinc

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case

os-image.bin: kernel.bin
	nasm -f bin boot/boot_loader.asm -o os-image.bin

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.asm
	nasm -f elf32 $< -o $@

run: os-image.bin
	qemu-system-i386 -serial stdio -drive format=raw,file=os-image.bin

run_term: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin -nographic -curses

kernel.bin: $(OBJS)
	$(LD) -o $@ -T linker.ld $^ --oformat binary

clean:
	rm -f */*.o
	rm -f *.bin

fclean: clean
	rm -f os-image.bin
