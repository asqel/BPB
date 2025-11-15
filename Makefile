
CC = gcc
LD = ld
CFLAGS = -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-stack-protector -fno-pie -g -I./include -nostdlib -nostdinc
LDFLAGS = --oformat=binary -T linker.ld -melf_i386 

KERNEL_SRC = $(wildcard src/*.c src/*.s)
KERNEL_OBJ = $(KERNEL_SRC:.c=.o)
KERNEL_OBJ := $(KERNEL_OBJ:.s=.o_s)
KERNEL = kernel.bin

TARGET = truc.iso

all: $(TARGET) run

$(TARGET): $(KERNEL) boot.s
	nasm -f bin boot.s -o $(TARGET)

$(KERNEL): $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o_s:%.s
	nasm -f elf32 $< -o $@

clean:
	rm -f $(KERNEL_OBJ)

fclean: clean
	rm -f $(KERNEL)
	rm -f $(TARGET)

re: fclean all

run:
	qemu-system-x86_64 -hda $(TARGET) -serial stdio

debug: $(TARGET)
	qemu-system-x86_64 -hda $(TARGET) -serial stdio -S -s
	

.PHONY: run re fclean clean all


