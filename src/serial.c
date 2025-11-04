#include <serial.h>
#include <ports.h>

void serial_init() {
	port_write8(SERIAL_PORT + 1, 0x00);
    port_write8(SERIAL_PORT + 3, 0x80);
    port_write8(SERIAL_PORT + 0, 0x03);
    port_write8(SERIAL_PORT + 1, 0x00);
    port_write8(SERIAL_PORT + 3, 0x03);
    port_write8(SERIAL_PORT + 2, 0xC7);
    port_write8(SERIAL_PORT + 4, 0x0B);
}

void serial_putc(char c) {
	while (!(port_read8(SERIAL_PORT + 5) & 0x20))
		;
	port_write8(SERIAL_PORT, c);
}

void serial_puts(char *str) {
	while (*str)
		serial_putc(*(str++));
}
