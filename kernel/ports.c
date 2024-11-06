
#include "kernel.h"

unsigned char port_byte_in (unsigned short port) {
    unsigned char res;
    asm volatile ("inb %1, %0" : "=a" (res) : "dN" (port));
    return res;
}

void 

void write_port_u8(int port, u8 val) {
    asm volatile ("outb %1, %0" : :  "dN" (port), "a" (val));
}