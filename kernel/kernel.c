#include <kernel.h>
#include <libft.h>
#include <oeuf.h>
#include <fal.h>

void close_os() {
    port_write_u16(0x604, 0x2000);   // qemu
    port_write_u16(0xB004, 0x2000);  // bochs
    port_write_u16(0x4004, 0x3400);  // virtualbox

    asm volatile("cli");
    asm volatile("hlt");
}

int olivine_main(int argc, char **argv);
int puts(const char *s);



#define grub_flag(info, N) (((info).flags >> N) & 0b1)

int check_graphics_mode(grub_info *info) {
    // Si le bit 12 (flags[12]) est activé, cela signifie qu'un framebuffer graphique est utilisé
    if (info->flags & (1 << 12)) {
        return 1; // Mode graphique
    } else {
        return 0; // Mode texte
    }
}


void game_main();


void hexdump(u8 *data, u32 len) {
    for (u32 i = 0; i < len; i++) {
        u8 hex_line[16] = {0};
        for (u32 k = i; k < i + 16 && k < len; k++) {
            hex_line[k - i] = data[k];
        }
        for (u32 k = 0; k < 16; k++)
            fprintf(serialout, "%x%x ", hex_line[k] >> 4, hex_line[k] &0x0f);
        fprintf(serialout, "    ");
        for (u32 k = 0; k < 16; k++) {
            if (hex_line[k] >= ' ' && hex_line[k] <= '~')
                fprintf(serialout, "%c", hex_line[k]);
            else
                fprintf(serialout, ".");
        }
        fprintf(serialout, "\n");
        i += 16;
    }
}

u8 *move_disk_to_heap(u8 *disk, u32 size) {
    extern u8 *heap;

    memmove(heap, disk, size);
    return malloc(size);
}

#define HEAP_ADDR 0x100000
#define HEAP_SIZE 0x100000

void init_idt();

extern void *kernel_start;
extern void *kernel_end;

void kernel_main(grub_info *info) {
    serial_init();

    u8 *disk = NULL;
    u32 disk_size = 0;
    if (info->mods_count > 0) {
        grub_module_t *disk_mod = &(((grub_module_t *)info->mods_addr)[0]);
        u8* disk_data = (u8 *)disk_mod->mod_start;
        disk_size = disk_mod->mod_end - disk_mod->mod_start;
        disk = heap_init_with_disk((u8 *)HEAP_ADDR, HEAP_SIZE, disk_data, disk_size);
        fal_init(disk, disk_size);
        fal_print_tree(&fal_root, 2, serialout);
    }
    else
        heap_init((u8 *)HEAP_ADDR, HEAP_SIZE);
    if (info->framebuffer_addr_low > 0xb800)
        //graphic_init(info); // !TODO

    init_gdt();
    init_idt();

    screen_clear();
    puts("it's a good idea to want to make an os that runs Windows exe and graphic driverslike to be able to run games (._.  )");
    puts("indeed it is\n");
    if (disk == NULL) {
        printf("\n\n\nABORT NO DISK FOUND\npress any key to exit\n");
        while (port_read_u8(0x60) == 0x1c);
        while (1) {
            if (port_read_u8(0x60) < 0x81)
                break;
        }
        close_os();
        return ;
    }
    rtc_init();
    ltds_timer_init();
    pci_init();
    pci_print();

    olivine_main(1, (char *[]){"olivine", NULL});
	while (1);
}


