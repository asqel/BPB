#include <stdarg.h>
#include <libft.h>
#include "oeuf.h"

static int putnbr_u32(u32 n, FILE *fd) {
    if (n == 0) {
    fputc('0', fd);
    return 1;
    }
    if (n < 10) {
        fputc('0' + n, fd);
        return 1;
    }
    int res = putnbr_u32(n / 10, fd) + 1;
    fputc('0' + (n % 10), fd);
    return res + 1;
}

static int putnbr_i32(i32 n, FILE *fd) {
    if (n >= 0)
        return putnbr_u32((u32) n, fd);
    if (n == -2147483648) {
        fwrite("-2147483648", 1, 11, fd);
        return 11;
    }
    else  {
        fputc('-', fd);
        int v = putnbr_u32((u32)-n, fd);
        return v + 1;
    }

}

/* support:
  + / space
*/
static int put_format_d(FILE *fd, va_list *args, oe_format_arg fm_arg) {
    i32 value = va_arg(*args, i32);
    int res = 0;
    if (value >= 0) {
        if (fm_arg.flag.plus) {
            fputc('+', fd);
            res++;
        }
        else if (fm_arg.flag.space) {
            fputc(' ', fd);
            res++;
        }
    }
    return putnbr_i32(value, fd) + res;
}

static int put_format_n(FILE *, va_list *args, oe_format_arg fm_arg) {
    int *ptr = va_arg(*args, int *);
    *ptr = fm_arg.current_count;
    return 0;
}

static int put_format_N(FILE *, va_list *, oe_format_arg) {
    return 0;
}

static int put_format_s(FILE *fd, va_list *args, oe_format_arg) {
    char *ptr = va_arg(*args, char *);
    int len = strlen(ptr);
    fwrite(ptr, 1, len, fd);
    return len;
}

static int puthex(u32 n, FILE *fd) {
    if (n < 16) {
        fwrite(&("0123456789ABCDEF"[n]), 1, 1, fd);
        return (1);
    }
    return puthex(n / 16, fd) + puthex(n % 16, fd);
}

static int put_format_x(FILE *fd, va_list *args, oe_format_arg) {
    u32 nbr = va_arg(*args, u32);
    return puthex(nbr, fd);
}

static int put_format_c(FILE *fd, va_list *args, oe_format_arg) {
    char c = va_arg(*args, int);
    return fputc(c, fd);
}

static int put_ptr(FILE *fd, u32 ptr) {
    if (ptr < 0x10)
        return fwrite(&("0123456789abcdef"[ptr]), 1, 1, fd);
    return put_ptr(fd, ptr / 0x10) + put_ptr(fd, ptr % 0x10);
}

static int put_format_p(FILE *fd, va_list *args, oe_format_arg) {
    return fwrite("0x", 1, 2, fd) + put_ptr(fd, (u32)va_arg(*args, void *));
}


oe_format_t blt_formats[] = {
    (oe_format_t){.func = &put_format_d, .specifier = "d"},
    (oe_format_t){.func = &put_format_n, .specifier = "n"},
    (oe_format_t){.func = &put_format_N, .specifier = "N"},
    (oe_format_t){.func = &put_format_N, .specifier = "M"},
    (oe_format_t){.func = &put_format_s, .specifier = "s"},
    (oe_format_t){.func = &put_format_x, .specifier = "x"},
    (oe_format_t){.func = &put_format_c, .specifier = "c"},
    (oe_format_t){.func = &put_format_p, .specifier = "p"},
};

int blt_formats_len = sizeof(blt_formats)/sizeof(blt_formats[0]);
