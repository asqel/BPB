

#include <kernel.h>

#define SCR_ADDR ((unsigned char *)0xb8000)

#define HEIGHT 25
#define WIDTH 80

#define SCR_AT(X, Y) (*(SCR_ADDR + 2 * ((X) + (Y) * WIDTH)))

static int x = 0;
static int y = 0;

static void scroll() {
	for (int i = 0; i < WIDTH; i++) {
		for (int k = 1; k < HEIGHT; k++) {
			SCR_AT(i, k - 1) = SCR_AT(i, k);
			*(1 +&SCR_AT(i, k - 1)) = *(1 +&SCR_AT(i, k));
		}
	}
	for (int i = 0; i < WIDTH; i++) {
		SCR_AT(i, HEIGHT - 1) = ' ';
		*(1 +&SCR_AT(i, HEIGHT - 1)) = 0;
	}
}

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void txt_cursor_blink(int on) {
    port_write_u8(REG_SCREEN_CTRL, 0x0A);
    port_write_u8(REG_SCREEN_DATA, (port_read_u8(REG_SCREEN_DATA) & 0xC0) | (on ? 0x0F : 0x0C));
}

void screen_set_cursor() {
    // similar to cursor_get_offset, but instead of reading we write data
	u16 offset = y * WIDTH + x;
    port_write_u8(REG_SCREEN_CTRL, 14);
    port_write_u8(REG_SCREEN_DATA, (u8)(offset >> 8));
    port_write_u8(REG_SCREEN_CTRL, 15);
    port_write_u8(REG_SCREEN_DATA, (u8)(offset & 0xff));
	txt_cursor_blink(0);
}

void screen_add_char(char c, char col) {
	if (c == '\b') {
		screen_erase();
		return ;
	}
	if (c == '\n') {
		y++;
		x = 0;
		if (y >= HEIGHT) {
			y--;
			scroll();
		}
		screen_set_cursor();
		return ;
	}
	SCR_AT(x, y) = c;
	*(1 + &SCR_AT(x, y)) = col;
	x++;
	if (x >= WIDTH) {
		x = 0;
		y++;
		if (y >= HEIGHT) {
			y--;
			scroll();
		}
	}
	screen_set_cursor();
}

void screen_erase() {
	x--;
	if (x < 0) {
		y--;
		if (y < 0)
			y = 0;
		x = WIDTH - 1;
		while (x >= 0 && SCR_AT(x, y) == 0)
			x--;
		if (x < 0)
			x = 0;
	}
	SCR_AT(x, y) = 0;
	*(1 + &SCR_AT(x, y)) = 0;
	screen_set_cursor();
}

void screen_clear() {
	x = 0;
	y = 0;
	for (int i = 0; i < WIDTH; i++) {
		for (int k = 0; k < HEIGHT; k++) {
			SCR_AT(i, k) = 0;
			*(1 + &SCR_AT(i, k)) = 0;
		}
	}
}

