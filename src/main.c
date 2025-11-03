
#define uint8_t unsigned char
#define uint16_t unsigned short int
#define uint32_t unsigned int
#define uint64_t unsigned long long int

int serial_write(uint32_t port, void *buffer, uint32_t len);
void serial_enable(int device);

char *screen = (char *)0x8b000;

void _start() {
	char c = 'A';
	serial_write(0x3f8, &c, 1);
	c= '\n';
	serial_write(0x3f8, &c, 1);
	while(1);
}

uint8_t port_read8(uint16_t port) {
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_write8(uint16_t port, uint8_t data) {
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_read16(uint16_t port) {
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_write16(uint16_t port, uint16_t data) {
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}

uint32_t port_read32(uint32_t port) {
    uint32_t result;
    asm volatile("inl %%dx,%%eax":"=a" (result):"d"(port));
    return result;
}

void port_write32(uint32_t port, uint32_t value) {
    asm volatile("outl %%eax,%%dx"::"d" (port), "a" (value));
}

void serial_enable(int device) {
    port_write8(device + 1, 0x00);
    port_write8(device + 3, 0x80); // enable divisor mode
    port_write8(device + 0, 0x03); // div Low:  03 Set the port to 38400 bps
    port_write8(device + 1, 0x00); // div High: 00
    port_write8(device + 3, 0x03);
    port_write8(device + 2, 0xC7);
    port_write8(device + 4, 0x0B);
}

int serial_write(uint32_t port, void *buffer, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        while (!(port_read8(port + 5) & 0x20))
            ;
        port_write8(port, ((char *) buffer)[i]);
    }
    return len;
}
