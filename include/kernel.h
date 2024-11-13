#ifndef KERNEL_H
#define KERNEL_H


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;

typedef __SIZE_TYPE__ size_t;

typedef i32 oe_arch_int;
typedef u32 oe_arch_uint;

typedef unsigned int uint;

typedef struct {
	int fd;
} FILE;

void serial_putc(char c);
void serial_putstr(char *s);
void serial_putnbr(int n);
void screen_clear();

void serial_init();
void screen_add_char(char c, char col);

#define SERIAL_PORT 0x3f8

u8 port_read_u8(u16 port);
void port_write_u8(u16 port, u8 val);
u16 port_read_u16(u16 port);
void port_write_u16(u16 port, u16 val);
void screen_erase();
void close_os();
int python_style_input(char *buffer, int max, char **history, int history_size);


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

#endif
