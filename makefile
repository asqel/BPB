LOADER_SRC = $(wildcard loader/*.c loader/*.asm)
LOADER_OBJS = ${LOADER_SRC:.c=.o} 
LOADER_OBJS := ${LOADER_OBJS:.asm=.o}

KERNEL_SRC = $(wildcard kernel/*.c kernel/*.asm )
KERNEL_OBJS = ${KERNEL_SRC:.c=.o} 
KERNEL_OBJS := ${KERNEL_OBJS:.asm=.o}


# Change this if your cross-compiler is somewhere else
CC = gcc -m32
LD = ld -m elf_i386
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-stack-protector -fno-pie -I./include -nostdlib -nostdinc -Os

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
os-image.bin: loader.bin  kernel.bin
	nasm -f bin boot/boot_loader.asm -o os-image.bin

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.asm
	nasm -f elf32 $< -o $@

run: os-image.bin
	qemu-system-i386 -serial stdio -drive format=raw,file=os-image.bin

run_term: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin -nographic -curses

loader.bin: $(LOADER_OBJS)
	$(LD) -o $@ -T loader_linker.ld $^ --oformat binary

kernel.bin: $(KERNEL_OBJS)
	$(LD) -o $@ -T kernel_linker.ld $^ --oformat binary

clean:
	rm -f */*.o
	rm -f *.bin

fclean: clean
	rm -f os-image.bin
