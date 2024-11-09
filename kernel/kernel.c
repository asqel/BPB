#include <kernel.h>

void close_os() {
    port_write_u16(0x604, 0x2000);   // qemu
    port_write_u16(0xB004, 0x2000);  // bochs
    port_write_u16(0x4004, 0x3400);  // virtualbox

    asm volatile("cli");
    asm volatile("hlt");
}

int olivine_main(int argc, char **argv);
int puts(const char *s);
void heap_init(void);

void kernel_main() {
    screen_clear();
    puts("it's a good idea to want to make an os that runs Windows exe and graphic driverslike to be able to run games (._.  )\n");
	serial_init();
    heap_init();

    olivine_main(1, (char *[]){"olivine"});
	while (1);
}
