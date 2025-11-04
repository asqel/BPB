#ifndef SERIAL_H
#define SERIAL_H

#define SERIAL_PORT 0x3f8

void serial_enable();
void serial_putc(char c);
void serial_puts(char *str);

#endif
