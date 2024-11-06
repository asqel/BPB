#ifndef KERNEL_H
#define KERNEL_H

void serial_putc(char c);
void serial_putstr(char *s);
void serial_putnbr(int n);

unsigned char port_byte_in (unsigned short _port);
void port_byte_out(int port, char b);

#define u8 	unsigned char
#define u16 short int
#define u32 int


#endif