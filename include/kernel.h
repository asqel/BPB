#ifndef KERNEL_H
#define KERNEL_H

#include <screen.h>

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

#define HEAP_SIZE 0x10000


// genre c'est standard
typedef struct tm {
    int    tm_sec;   // seconds [0,61]
    int    tm_min;   // minutes [0,59]
    int    tm_hour;  // hour [0,23]
    int    tm_mday;  // day of month [1,31]
    int    tm_mon;   // month of year [0,11]
    int    tm_year;  // years since 1900
    int    tm_wday;  // day of week [0,6] (Sunday = 0)
    int    tm_yday;  // day of year [0,365]
    int    tm_isdst; // daylight savings flag
} tm_t;

typedef struct {
	int fd;
} FILE;

#define stdout ((FILE *)1)
#define serialout ((FILE *)2)
#define stderr stdout
#define stdin stdout

#define SERIAL_PORT 0x3f8

void serial_init();
void serial_putc(char c);
void serial_putstr(char *s);
void serial_putnbr(int n);
void screen_clear();

void timer_init(void);
u32  timer_sleep(u32 ms);
int  time_get(tm_t *target);

u8   port_read_u8(u16 port);
void port_write_u8(u16 port, u8 val);
u16  port_read_u16(u16 port);
void port_write_u16(u16 port, u16 val);

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

#endif
