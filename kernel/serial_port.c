
#include "kernel.h"

void serial_putc(char c) {
    while (!(port_read_u8(SERIAL_PORT + 5) & 0x20));
    port_write_u8(0x3f8, c);
}
void serial_putstr(char *s) {
    while (*s)
        serial_putc(*(s++));
}

void serial_putnbr(int n) {
    if (n ==  (int)0x80000000) {
        serial_putstr("-2147483648");
        return ;
    }
    if (n < 0) {
        serial_putc('-');
        serial_putnbr(-n);
        return ;
    }
    if (n == 0) {
        serial_putc('0');
        return;
    }
    if (n < 10) {
        serial_putc('0' + n);
        return;
    }
    serial_putnbr(n / 10);
    serial_putnbr(n % 10);
}

void serial_init() {
    port_write_u8(SERIAL_PORT + 1, 0x00);
    port_write_u8(SERIAL_PORT + 3, 0x80); 
    port_write_u8(SERIAL_PORT + 0, 0x03);
    port_write_u8(SERIAL_PORT + 1, 0x00); 
    port_write_u8(SERIAL_PORT + 3, 0x03);
    port_write_u8(SERIAL_PORT + 2, 0xC7);
    port_write_u8(SERIAL_PORT + 4, 0x0B);
}