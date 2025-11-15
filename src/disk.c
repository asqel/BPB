#include <disk.h>

uint8_t driver_number = 0xFF;

void init_disk() {
	driver_number = *(uint8_t *)(0x7c00 + 512 + 2);
}
