#include <kernel.h>
#include <oeuf.h>
#include <libft.h>

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

int ata_read_sector(unsigned int lba, unsigned short int* buffer);

void kernel_main(u32 disk_size, u32 disk_start) {
    screen_clear();
    puts("it's a good idea to want to make an os that runs Windows exe and graphic driverslike to be able to run games (._.  )\n");
    serial_init();
    heap_init();

    int i = 0;
    u8 buffer[256 * 4];
    char *signature = "$$@@$$^^%%^^***?**?**?*(())BPB DISK\n";
    int len = strlen(signature);
    char *to_search = malloc(len* 4 +1);
    to_search[0] = '\0';
    strcat(to_search, signature);
    strcat(to_search, signature);
    strcat(to_search, signature);
    strcat(to_search, signature);
    int found = 0;
    int sector = 0;
    int sub_Sector = 0;
    while (found == 0) {
        if (ata_read_sector(i, buffer) == 0) break;
        if (ata_read_sector(i + 1, &(buffer[256])) == 0) break;
        for (int k = 0; k < 512 * 2 - len + 1; k++) {
            if (!strcmp(to_search, ((u8 *)buffer) + k)) {
                found  = 1;
                sector = i;
                sub_Sector = k;
                break;
            }
        }
        i++;
    }
    if (found) {
        printf("found disk in sector %d:%d\n", sector, sub_Sector);
    }

    olivine_main(1, (char *[]){"olivine"});
	while (1);
}
