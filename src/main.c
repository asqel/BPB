#include <ports.h>
#include <disk.h>
#include <serial.h>

char *screen = (char *)0xb8000;
int x = 0;
int y = 0;

#define  HEIGHT 25
#define WIDTH 80


void putc(char c) {
	if (c == '\n') {
		y++;
		x = 0;
	}
	else {
		screen[(x + y * WIDTH) * 2] = c;
		screen[(x + y * WIDTH) * 2 + 1] = 0x0f;
		x++;
		if (x >= WIDTH) {
			x = 0;
			y++;
		}
	}
}
void putstr(char *str) {
	while (*str)
		putc(*(str++));
}

void putnbr(uint32_t n) {
	if (n == 0)
		putc('0');
	else if (n < 16) {
		if (n < 10)
			putc(n + '0');
		else
			putc(n - 10 + 'a');
	}
	else {
		putnbr(n / 16);
		putnbr(n % 16);
	}
}

void kernel_main() {
	/*serial_init();
	init_disk();
	uint16_t *screen2 = (uint16_t *)screen;
	for (int i = 0; i < 80 * 25; i++) {
		screen2[i] = 0;
	}
	putnbr(*(uint8_t *)(0x7c00 + 512));
	putc(' ');
	putnbr(*(uint16_t *)(0x7c00 + 512 + 1));*/

	uint8_t *screen = 0xb8000;
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		*screen = i;
		screen++;
		*screen = 0x0f;
		screen++;
	}


	while(1);
}
