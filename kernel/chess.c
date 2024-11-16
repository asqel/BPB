#include <kernel.h>
#include <screen.h>
#include <oeuf.h>


enum {
    CHES_NONE,
    CHESS_PAWN,
    CHESS_TOWER,
    CHESS_KNIGHT,
    CHESS_BISHOP,
    CHESS_KING,
    CHESS_QUEEN,
    CHESS_MAX
};

static char *pieces_textures[][2] = {
	{NULL, NULL},
	{
		"()",
		"]["
	},
	{
		"^^",
		"[]"
	},
	{
		"*|",
		"|/"
	},
	{
		"/\\",
		"]["
	},
	{
		"++",
		"]["
	},
	{
		"++",
		"/\\"
	}
};

static void draw_piece(int x, int y, u8 piece) {
	for (int i = 0; i < 2; i++)
		for (int k = 0; k < 2; k++) {
			SCR_AT(x + k, y + i) = pieces_textures[piece][i][k];
		}
}

static void chess_draw(u8 board[8][8], int x_offset, int y_offset) {
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            int x = x_offset + k * 2;
            int y = y_offset + i * 2;
            u8 col = 0;

            if ((i + k) % 2 == 0)
                col |= VGA_LIGHT_GRAY << 4; // white slot
            else
                col |= VGA_GREEN << 4;

            if (board[i][k] != CHES_NONE) {
                if (board[i][k] & 0b10000000) // is white piece
                    col |= VGA_WHITE;
                else
                    col |= VGA_BLACK;
				draw_piece(x, y, board[i][k] & 0b01111111);
            }
			else {
                if ((i + k) % 2 == 0)
                    col |= VGA_LIGHT_GRAY;
                else
                    col |= VGA_GREEN;
            }
            for (int dx = 0; dx < 2; dx++) {
                for (int dy = 0; dy < 2; dy++) {
                    SCR_COL(x + dx, y + dy) = col;
                }
            }
        }
    }
}

void chess_main() {
    u8 board[8][8] = {
        {CHESS_TOWER | 0b10000000, CHESS_KNIGHT | 0b10000000, CHESS_BISHOP | 0b10000000, CHESS_QUEEN | 0b10000000,
         CHESS_KING | 0b10000000, CHESS_BISHOP | 0b10000000, CHESS_KNIGHT | 0b10000000, CHESS_TOWER | 0b10000000},
        {CHESS_PAWN | 0b10000000, CHESS_PAWN | 0b10000000, CHESS_PAWN | 0b10000000, CHESS_PAWN | 0b10000000,
         CHESS_PAWN | 0b10000000, CHESS_PAWN | 0b10000000, CHESS_PAWN | 0b10000000, CHESS_PAWN | 0b10000000},
        {CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE},
        {CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE},
        {CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE},
        {CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE, CHES_NONE},
        {CHESS_PAWN, CHESS_PAWN, CHESS_PAWN, CHESS_PAWN, CHESS_PAWN, CHESS_PAWN, CHESS_PAWN, CHESS_PAWN},
        {CHESS_TOWER, CHESS_KNIGHT, CHESS_BISHOP, CHESS_QUEEN, CHESS_KING, CHESS_BISHOP, CHESS_KNIGHT, CHESS_TOWER}
    };
    chess_draw(board, 0, 0);
}
