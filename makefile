LOADER_SRC = $(wildcard loader/*.c loader/*.asm)
LOADER_OBJS = ${LOADER_SRC:.c=.o} 
LOADER_OBJS := ${LOADER_OBJS:.asm=.o}

KERNEL_SRC = $(wildcard kernel/*.c libft/*.c oeuf/*.c)
KERNEL_OBJS = ${KERNEL_SRC:.c=.o} 
KERNEL_OBJS := ${KERNEL_OBJS:.asm=.o}

# Change this if your cross-compiler is somewhere else
CC = gcc -m32
LD = ld -m elf_i386

CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-stack-protector -fno-pie -I./include -nostdlib -nostdinc

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
BPB.bin: loader.bin kernel.bin
	nasm -f bin boot/boot_loader.asm -o BPB.bin

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.asm
	nasm -f elf32 $< -o $@

run: BPB.bin
	qemu-system-i386 -serial stdio -drive format=raw,file=BPB.bin

run_term: BPB.bin
	qemu-system-i386 -drive format=raw,file=BPB.bin -nographic -curses

run_grub: iso
	qemu-system-i386 -drive format=raw,file=BPB.iso

loader.bin: $(LOADER_OBJS)
	$(LD) -o $@ -T build/loader.ld $^ --oformat binary

kernel.bin: build/entry_local.o $(KERNEL_OBJS)
	$(LD) -o $@ -T build/kernel_local.ld $^ --oformat binary

kernel.elf: build/entry_grub.o $(KERNEL_OBJS)
	$(LD) -o $@ -T build/kernel_grub.ld $^

iso: kernel.elf
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/kernel.elf
	cp build/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o BPB.iso isodir
	rm -rf isodir

clean:
	find . -name '*.o' -delete
	rm -f *.bin *.iso *.elf
