#ifndef SCREEN_H
#define SCREEN_H

#define SCR_ADDR ((unsigned char *) 0xb8000)

#define HEIGHT 25
#define WIDTH 80

#define VGA_BLACK         0x0
#define VGA_BLUE          0x1
#define VGA_GREEN         0x2
#define VGA_CYAN          0x3
#define VGA_RED           0x4
#define VGA_MAGENTA       0x5
#define VGA_BROWN         0x6
#define VGA_LIGHT_GRAY    0x7
#define VGA_DARK_GRAY     0x8
#define VGA_LIGHT_BLUE    0x9
#define VGA_LIGHT_GREEN   0xA
#define VGA_LIGHT_CYAN    0xB
#define VGA_LIGHT_RED     0xC
#define VGA_LIGHT_MAGENTA 0xD
#define VGA_YELLOW        0xE
#define VGA_WHITE         0xF

#define SCR_AT(X, Y) (*(SCR_ADDR + 2 * ((X) + (Y) * WIDTH)))
#define SCR_COL(X, Y) (*(1 + SCR_ADDR + 2 * ((X) + (Y) * WIDTH)))

void screen_erase(void);
void screen_add_char(char c, char col);

#endif
