#include <kernel.h>
#include <oeuf.h>

u8 *screen_fb = 0;
u32 screen_pitch = 0;
u32 screen_height = 0;
u32 screen_width = 0;
u32 screen_byte_pp = 0;

static u32 font_size = 1;
static u32 font_width = 20;
static u32 font_height = 30;
static u32 char_per_line = 0;
static u32 nbr_line = 0;
static u32 **screen_chars = NULL;

u8 is_graphic_mode = 0;

void graphic_init(grub_info *info) {
	screen_fb = info->framebuffer_addr_low;
	screen_pitch = info->framebuffer_pitch;
	screen_height = info->framebuffer_height;
	screen_width = info->framebuffer_width;
	screen_byte_pp = info->framebuffer_bpp / 8;

    char_per_line = screen_width / font_width;
    nbr_line = screen_height / font_height;
    screen_chars = malloc(sizeof(u32 *) * nbr_line);
    for (int i = 0; i < nbr_line; i++)
        screen_chars[i] = malloc(sizeof(u32) * char_per_line);
    is_graphic_mode = 1;
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
	int real_x = x * font_width * font_size;
	int real_y = y * font_height * font_size;
	draw_glyph_at(font[c], std_font_size, real_x, real_y);
}


void update_screen() {
    for (int x = 0; x < char_per_line; x++)
        for (int y = 0; y < nbr_line; y++)
            draw_std_char(screen_chars[y][x], x, y);
}