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
    return 0;
}

oe_format_t blt_formats[] = {
    (oe_format_t){.func = &put_format_d, .specifier = "d"},
    (oe_format_t){.func = &put_format_n, .specifier = "n"},
    (oe_format_t){.func = &put_format_N, .specifier = "N"},
    (oe_format_t){.func = &put_format_N, .specifier = "M"},
    (oe_format_t){.func = &put_format_s, .specifier = "s"},
};

int blt_formats_len = sizeof(blt_formats)/sizeof(blt_formats[0]);
