#ifndef SCREEN_H
#define SCREEN_H


#include <stdint.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

extern int screen_x;
extern int screen_y;
extern uint16_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];

void putc(char c, uint8_t col);

#endif
