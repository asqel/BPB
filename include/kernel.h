#ifndef KERNEL_H
#define KERNEL_H


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;

typedef __SIZE_TYPE__ size_t;

typedef i32 oe_arch_int;
typedef u32 oe_arch_uint;

typedef unsigned int uint;

typedef struct {
	int fd;
} FILE;

void serial_putc(char c);
void serial_putstr(char *s);
void serial_putnbr(int n);
void screen_clear();

void serial_init();
void screen_add_char(char c, char col);

#define SERIAL_PORT 0x3f8

u8 port_read_u8(u16 port);
void port_write_u8(u16 port, u8 val);
u16 port_read_u16(u16 port);
void port_write_u16(u16 port, u16 val);
void screen_erase();
void close_os();
int python_style_input(char *buffer, int max, char **history, int history_size);

#endif
