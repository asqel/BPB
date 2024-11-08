#include "kernel.h"

char keyboard_map[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '!', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '(', ')',
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9',
	'-', '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

int python_style_input(char *buffer, int max, char **history, int history_size) {
    unsigned char c, old = 0;
    int history_index = history_size + 1;
    int current = 0;
    buffer[0] = 0;

    while (port_read_u8(0x60) == 0x1c);

    while (1) {
        c = port_read_u8(0x60);
        if (c == old)
            continue;
        old = c;

        if (c == 0x01)      // esc
            close_os();

        if (c == 0x1c) {    // enter
            screen_add_char('\n', 0x0f);
            return current;
        }

        if (c == 0x0e) {    // backspace
            if (current > 0) {
                screen_erase();
                current--;
                buffer[current] = 0;
            }
            continue;
        }

        if (c == 0x48 && history) {    // up
            if (history_index > 1) {
                history_index--;
                for (int i = 0; i < current; i++) {
                    screen_erase();
                }
                current = 0;
                while (history[history_index][current]) {
                    buffer[current] = history[history_index][current];
                    screen_add_char(buffer[current], 0x0A);
                    current++;
                }
                buffer[current] = 0;
            }
            continue;
        }

        if (c == 0x50 && history) {    // down
            if (history_index <= history_size) {
                history_index++;
                for (int i = 0; i < current; i++) {
                    screen_erase();
                }
                current = 0;
                while (history[history_index][current]) {
                    buffer[current] = history[history_index][current];
                    screen_add_char(buffer[current], 0x0A);
                    current++;
                }
                buffer[current] = 0;
            }
            if (history_index == history_size + 1) {
                for (int i = 0; i < current; i++) {
                    screen_erase();
                }
                buffer[0] = 0;
                current = 0;
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
