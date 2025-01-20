DIRS = $(wildcard *.dir)
OUTS = $(DIRS:.dir=)

CC = gcc -m32 -ffreestanding -nostdlib -nostdinc

all: out/ $(OUTS)

_start_file.o: _start_file.c
	$(CC) -c _start_file.c -o _start_file.o

out/:
	mkdir -p out/

%: %.dir
	$(CC) _start_file.o $(shell find $< -name "*.c") -o out/$@ $(shell cat $</flags)

clean:
	rm -f $(OUTS)

fclean: clean

.PHONY: clean fclean all