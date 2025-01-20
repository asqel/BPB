#include <kernel.h>

/*
toa (The One Above)

elf32 program launcher

*/

static u8 *magic_number = {0x7F, 'E', 'L', 'F'};
static u32 magic_len = 4;

enum TOA_ERRORS{
	TOA_ERR_NONE,
	TOA_ERR_NOT_ELF
};

// returns 0 if no error else an error code
int toa_launch(u8 *prog, u32 len, int *main_return) {
	for (u32 i = 0; i < magic_len; i++)
		if (prog[i] != magic_number[i])
			return TOA_ERR_NOT_ELF;
	return TOA_ERR_NONE;
}