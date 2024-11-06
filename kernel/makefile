SRCS = $(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c) $(wildcard *.asm) $(wildcard */*.asm) $(wildcard */*/*.asm)
OBJS = ${SRCS:.c=.o}
OBJS := ${OBJS:.asm=.o}
CC = gcc
LD = ld -m elf_i386
CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-stack-protector -fno-pie

all: kernel.bin

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.asm
	nasm -f elf32 $< -o $@

kernel.bin: $(OBJS)
	$(LD) -o ../$@ -T linker.ld $^ --oformat binary

clean:
	rm -rf *.o ../kernel.bin
