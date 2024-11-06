
#include "kernel.h"

u8 port_read_u8(u16 port) {
    unsigned char res;
    asm volatile ("inb %1, %0" : "=a" (res) : "dN" (port));
    return res;
}

void port_write_u8(u16 port, u8 val) {
    asm volatile ("outb %1, %0" : :  "dN" (port), "a" (val));
}

u16 port_read_u16(u16 port) {
    u16 result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_write_u16(u16 port, u16 val) {
    asm volatile("out %%ax, %%dx" : : "a" (val), "d" (port));
}
