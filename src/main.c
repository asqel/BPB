#include <ports.h>

int serial_write(uint32_t port, void *buffer, uint32_t len);
void serial_enable(int device);

char *screen = (char *)0xb8000;

void putc(char c) {
	*(screen++) = c;
	*(screen++) = 0x0f;
}
void putstr(char *str) {
	while (*str)
		putc(*(str++));
}

void kernel_main() {
	uint16_t *screen2 = (uint16_t *)screen;
	for (int i = 0; i < 80 * 25; i++) {
		screen2[i] = (0x0f << 8) | i;
	}
	while(1);
}
