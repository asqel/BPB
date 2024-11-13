#include <kernel.h>
#include <oeuf.h>

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



#define grub_flag(info, N) (((info).flags >> N) & 0b1)

int check_graphics_mode(grub_info *info) {
    // Si le bit 12 (flags[12]) est activé, cela signifie qu'un framebuffer graphique est utilisé
    if (info->flags & (1 << 12)) {
        return 1; // Mode graphique
    } else {
        return 0; // Mode texte
    }
}

extern u32 font[256][30];

void draw_glyph_at(u32 *glyphe, int size, int x, int y);
void graphic_init(grub_info *);
void draw_std_char(char c, int x, int y);

void game_main();

void kernel_main(grub_info *info) {

    screen_clear();
    puts("it's a good idea to want to make an os that runs Windows exe and graphic driverslike to be able to run games (._.  )");
    puts("indeed it is\n");

    serial_init();
    rtc_init();

    timer_init();

    serial_putnbr(info->vbe_mode_info);
    heap_init();
   //graphic_init(info);
   //if (info->framebuffer_addr_low > 0xb800) {
   //    draw_std_char('$', 0 , 10);
   //    draw_std_char('$', 10 , 0);
   //}

    olivine_main(1, (char *[]){"olivine"});
	while (1);
}


