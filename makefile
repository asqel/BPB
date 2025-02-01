KERNEL_SRC = $(shell find kernel libft oeuf kernel -type f -name '*.c' -o -name '*.asm')
KERNEL_SRC += build/entry_grub.asm
KERNEL_OBJS = ${KERNEL_SRC:.c=.o}
KERNEL_OBJS := ${KERNEL_OBJS:.asm=.o_s}

# Change this if your cross-compiler is somewhere else
CC = gcc -m32
LD = ld -m elf_i386

CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-stack-protector -fno-pie -g -I./include -nostdlib -nostdinc

all: run

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
run: BPB.iso
	qemu-system-i386 -drive format=raw,file=BPB.iso -serial stdio #-d cpu_reset

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o_s: %.asm
	nasm -f elf32 $< -o $@


run_term: BPB.bin
	qemu-system-i386 -drive format=raw,file=BPB.bin -nographic -curses

kernel.bin: build/entry_local.o $(KERNEL_OBJS)
	$(LD) -o $@ -T build/kernel_local.ld $^ --oformat binary

kernel.elf: $(KERNEL_OBJS)
	$(LD) -o $@ -T build/kernel_grub.ld $^

BPB.iso: kernel.elf disk
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/kernel.elf
	cp disk.bin isodir/boot/disk.bin
	cp build/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o BPB.iso isodir
	rm -rf isodir

disk:
	make -C compile_env
	mkdir -p disk/cmd
	cp compile_env/out/* disk/cmd/
	python3 disk.py

clean:
	find . -name '*.o' -delete
	rm -f *.bin *.iso *.elf

re: clean all

.PHONY: run run_term clean disk re
