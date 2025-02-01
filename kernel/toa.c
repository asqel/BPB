#include <kernel.h>

/*
toa (The One Above)

elf32 program launcher
(only shared object containing _start)

*/

# define ELF_NIDENT	16

typedef u16 Elf32_Half;	// Unsigned half int
typedef u32 Elf32_Off;	// Unsigned offset
typedef u32 Elf32_Addr;	// Unsigned address
typedef u32 Elf32_Word;	// Unsigned int
typedef i32 Elf32_Sword;// Signed int

typedef struct {
	u8 e_ident[ELF_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
} Elf32_Ehdr;

static u8 magic_number[4] = {0x7F, 'E', 'L', 'F'};
static u32 magic_len = 4;

enum TOA_ERRORS {
	TOA_ERR_NONE,
	TOA_ERR_NOT_ELF,
	TOA_ERR_UNSUPPORTED
};

// returns 0 if no error else an error code
int toa_launch(u8 *prog, u32 len, int *main_return) {
	if (len < magic_len)
		return TOA_ERR_NOT_ELF;
	for (u32 i = 0; i < magic_len; i++) {
		if (prog[i] != magic_number[i])
			return TOA_ERR_NOT_ELF;
	}
	fprintf(serialout, "machine: %d\n", ((Elf32_Ehdr *)prog)->e_machine);
	return TOA_ERR_NONE;
}


int toa_launch_aout(u8 *prog, u32 len) {
	return -1;
}