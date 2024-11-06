#ifndef KERNEL_H
#define KERNEL_H

#define u8 	unsigned char
#define u16 unsigned short int
#define u32 int


void serial_putc(char c);
void serial_putstr(char *s);
void serial_putnbr(int n);

void serial_init();
void screen_add_char(char c, char col);

#define SERIAL_PORT 0x3f8

u8 port_read_u8(u16 port);
void port_write_u8(u16 port, u8 val);
u16 port_read_u16(u16 port);
void port_write_u16(u16 port, u16 val);
void screen_erase();

#endif
