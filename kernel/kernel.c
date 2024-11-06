
#include "kernel.h"



void kernel_main(struct mode_info *info) {
 
    serial_putc('S');
    for (int i = 0; i < 80; i++) {
    *((char *)0xb8000  + i*2) = 'A';
    
    }
    while(1);
}
