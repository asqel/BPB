#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#define DISK_BUFFER_ADDR 0x7c00

extern uint8_t drive_number;

void init_disk();

#endif
