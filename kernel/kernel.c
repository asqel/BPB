
#include "kernel.h"

int seed = 0;

void screen_puts(char *string) {
    for (int i = 0; string[i] != 0; i++) {
        screen_add_char(string[i], 0x0f);
    }
}

int random() {
    unsigned int next = seed;
    int result;

    next *= 1103515245;
    next += 12345;
    result = (unsigned int) (next / 65536) % 2048;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int) (next / 65536) % 1024;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int) (next / 65536) % 1024;

    seed = next;
    return result;
}

char keyboard_map[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9',
	'-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64

void ps2_init() {
    port_write_u8(PS2_COMMAND_PORT, 0xAD); // Désactiver le port PS/2 1
    port_write_u8(PS2_COMMAND_PORT, 0xA7); // Désactiver le port PS/2 2 (si présent)

    // Vider le tampon
    while (port_read_u8(PS2_STATUS_PORT) & 1) {
        port_read_u8(PS2_DATA_PORT);
    }

    // Activer les périphériques
    port_write_u8(PS2_COMMAND_PORT, 0xAE); // Activer le port PS/2 1
    port_write_u8(PS2_COMMAND_PORT, 0xA8); // Activer le port PS/2 2 (si présent)
}

void close_os() {
    port_write_u16(0x604, 0x2000);   // qemu
    port_write_u16(0xB004, 0x2000);  // bochs
    port_write_u16(0x4004, 0x3400);  // virtualbox

    asm volatile("cli");
    asm volatile("hlt");
}

int python_style_input(char *buffer, int max) {
    unsigned char c, old;
    int current = 0;
    buffer[0] = 0;

    screen_puts(">>> ");

    while (1) {
        c = port_read_u8(0x60);
        if (c == old)
            continue;
        old = c;

        if (c == 0x01)
            close_os();

        if (c == 0x1c) {
            while (port_read_u8(0x60) == 0x1c);
            return current;
        }

        if (c == 0x0e) {
            if (current > 0) {
                screen_erase();
                current--;
                buffer[current] = 0;
            }
            continue;
        }
        
        if (c < 128 && current < max - 1) {
            buffer[current] = keyboard_map[c];
            if (buffer[current] == 0)
                continue;
            screen_add_char(buffer[current], 0x0A);
            buffer[++current] = 0;
        }
    }
}

void kernel_main(void) {
    serial_init();
    ps2_init();

    char buffer[256];

    while(1) {
        python_style_input(buffer, 256);
        screen_puts("\noui: ");
        screen_puts(buffer);
        screen_puts("\n");
    }
}
