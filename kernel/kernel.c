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

typedef struct {
    u32 flags;

    // if flags[0]
    u32 mem_lower;
    u32 mem_upper;

    // if flags[1]
    u32 boot_device;

    // if flags[2]
    u32 cmdline;

    // if flags[3]
    u32 mods_count;
    u32 mods_addr;

    // if flags[4] || flags[5]
    union {
        struct {
            u32 tabsize;
            u32 strsize;
            u32 addr;
            u32 reserved;
        } aout_sym;
        struct {
            u32 num;
            u32 size;
            u32 addr;
            u32 shndx;
        } elf_sec;
    } syms;

    // if flags[6]
    u32 mmap_length;
    u32 mmap_addr;

    // if flags[7]
    u32 drives_length;
    u32 drives_addr;

    // if flags[8]
    u32 config_table;

    // if flags[9]
    u32 boot_loader_name;

    // if flags[10]
    u32 apm_table;

    //  if flags[11]
    u32 vbe_control_info;
    u32 vbe_mode_info;
    u16 vbe_mode;
    u16 vbe_interface_seg;
    u16 vbe_interface_off;
    u16 vbe_interface_len;

    // if  flags[12]
    u32 framebuffer_addr_low;
    u32 framebuffer_addr_high;
    u32 framebuffer_pitch;
    u32 framebuffer_width;
    u32 framebuffer_height;
    u8 framebuffer_bpp;
    u8 framebuffer_type;
    u8 color_info[6];
} grub_info;

int check_graphics_mode(grub_info *info) {
    // Si le bit 12 (flags[12]) est activé, cela signifie qu'un framebuffer graphique est utilisé
    if (info->flags & (1 << 12)) {
        return 1; // Mode graphique
    } else {
        return 0; // Mode texte
    }
}

void kernel_main(grub_info *info) {
    screen_clear();
    puts("it's a good idea to want to make an os that runs Windows exe and graphic driverslike to be able to run games (._.  )\n");
    serial_init();
    serial_putnbr(info->vbe_mode_info);
    heap_init();
    olivine_main(1, (char *[]){"olivine"});
	while (1);
}
