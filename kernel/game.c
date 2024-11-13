#include <kernel.h>

struct obj_t;
struct chunk_t;
struct player_t;


typedef struct {
	u16 id;
	u8 color;
	void *on_interact;
	u8 colision;
} obj_t;

typedef struct  {
	obj_t objs[20][40];

} chunk_t;

chunk_t test_chunk = {.objs = {0}};

typedef struct  {
	int x;
	int y;
	int x_in_chunk;
	int y_in_chunk;
	u8 dir; // 0 up, 1 right, 2 down, 3 left
} player_t;


void render_chunk(chunk_t chunk, player_t player) {
	screen_clear();
	for (int i = 0; i < 40; i++)
		for (int k = 0; k < 20; k++) {
			int x = WIDTH / 2 - 40 / 2 + i;
			int y = k;
			SCR_AT(x, y) = ' ';
			*(1 + &SCR_AT(x, y)) = chunk.objs[k][i].color;
	}
	SCR_AT(WIDTH / 2 - 40 / 2 + player.x_in_chunk, player.y_in_chunk) = '!';
	*(1 + &SCR_AT(WIDTH / 2 - 40 / 2 + player.x_in_chunk, player.y_in_chunk)) = 0x0f;
	for (int i  = 0; i < 20; i++) {
		int x  = WIDTH / 2 - 40 / 2 - 1;
		SCR_AT(x, i) = ' ';
		*(1 + &SCR_AT(x, i)) = VGA_WHITE << 4;
		x  = WIDTH / 2 - 40 / 2 + 40;
		SCR_AT(x, i) = ' ';
		*(1 + &SCR_AT(x, i)) = VGA_WHITE << 4;
	}
	for (int i = 0; i < 42; i++) {
		int x  = WIDTH / 2 - 40 / 2 - 1 + i;
		SCR_AT(x, 20) = ' ';
		*(1 + &SCR_AT(x, 20)) = VGA_WHITE << 4;
	}
}

extern char keyboard_map[];

#define PIT_FREQUENCY 1193182  // Fréquence de base du PIT
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_DATA_PORT 0x40

void pit_sleep(u32 ms) {
    u16 divisor = (PIT_FREQUENCY / 1000) * ms;

    // Configure le PIT en mode 0, avec un compteur de `divisor`
    port_write_u8(PIT_COMMAND_PORT, 0x36);
    port_write_u8(PIT_CHANNEL0_DATA_PORT, divisor & 0xFF);
    port_write_u8(PIT_CHANNEL0_DATA_PORT, (divisor >> 8) & 0xFF);

    // Boucle d'attente en synchronisation avec le PIT
    u32 ticks = 0;
    while (ticks < ms) {
        ticks++;
        for (u32 i = 0; i < 1000; i++) { asm volatile("nop"); }  // Delay approximatif
    }
}

void render_info(player_t player) {
	int cross_x = 0;;
	int cross_y = 0;
	u8 cross[3][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}};
	cross[0][1] = (player.dir == 0);
	cross[1][2] = (player.dir == 1);
	cross[2][1] = (player.dir == 2);
	cross[1][0] = (player.dir == 3);
	for (int i = 0; i < 3; i++)
		for (int k = 0; k < 3; k++) {
			int x = cross_x + k;
			int y = cross_y + i;
			if (i == 1 && k == 1) {
				SCR_AT(x, y) = ' ';
				*(1 + &SCR_AT(x, y)) = VGA_LIGHT_GRAY << 4;
			}
			if (cross[i][k] == 1)  {
				SCR_AT(x, y) = ' ';
				*(1 + &SCR_AT(x, y)) = VGA_GREEN << 4;
			}
			else if (cross[i][k] == 0) {
				SCR_AT(x, y) = ' ';
				*(1 + &SCR_AT(x, y)) = VGA_RED << 4;
			}
		}
}

void game_main() {
	screen_clear();
	player_t player = {0};

	char old = -1;

    while (1) {
		while (port_read_u8(0x60) == 0x1c);

       	char c = port_read_u8(0x60);
        if (c == old)
            continue;
        old = c;

		if (c == 59)
			return ;
		if (c == 0x11) {
			player.y_in_chunk -= 1;
			player.dir = 0;
		}
		if (c == 0x1f) {
			player.y_in_chunk += 1;
			player.dir = 2;
		}
		if (c == 0x1e) {
			player.x_in_chunk -= 1;
			player.dir = 3;
		}
		if (c == 0x20) {
			player.x_in_chunk += 1;
			player.dir = 1;
		}

		if (player.x_in_chunk < 0)
			player.x_in_chunk = 0;
		if (player.x_in_chunk >= 40)
			player.x_in_chunk = 40 - 1;

		if (player.y_in_chunk < 0)
			player.y_in_chunk = 0;
		if (player.y_in_chunk >= 20)
			player.y_in_chunk = 20 - 1;

		render_chunk(test_chunk, player);
		render_info(player);
		pit_sleep(1000);
	}

}

