#include <kernel.h>

u8 *screen_fb = 0;
u32 screen_pitch = 0;
u32 screen_height = 0;
u32 screen_width = 0;
u32 screen_byte_pp = 0;

void graphic_init(grub_info *info) {
	screen_fb = info->framebuffer_addr_low;
	screen_pitch = info->framebuffer_pitch;
	screen_height = info->framebuffer_height;
	screen_width = info->framebuffer_width;
	screen_byte_pp = info->framebuffer_bpp / 8;
}



void draw_square(int x, int y, int col, int size) {
    u32 real_col = 0;
    if (col)
        real_col = 0xffffffff;
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            *(u32 *)&screen_fb[(x +i) * 4 + (y + k) * screen_pitch] = real_col;
        }
    }
}

void draw_glyph_at(u32 *glyphe, int size, int _x, int _y) {

    for (int x = 0; x < 20; x++)
        for (int y = 0; y < 30; y++) {
            draw_square(_x + x * size, _y + y * size, (glyphe[y] >> (20  - x)) & 1, size);
        }
}

int std_font_size = 1;

extern u32 font[256][30];

void draw_std_char(char c, int x, int y) {
	int real_x = x * 20 * std_font_size;
	int real_y = y * 30 * std_font_size;
	draw_glyph_at(font[c], std_font_size, real_x, real_y);
}