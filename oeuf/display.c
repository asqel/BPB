#include <kernel.h>
#include <libft.h>
#include <stdarg.h>
#include "oeuf.h"


#ifndef elif
#define elif else if
#endif

oe_format_flag null_flag = {0};


/*


printf format

%M use array for print
    int *r = malloc(sizeof(int) * 2);
    r[0] = 1;
    r[1] = 2;
    printf("%M%d %d", 2, r);
    %M linked to the 2 (length of the array)
    and will take the content of   r as argument for print

%N same as %n but doesn't take argument from va_arg

*/


//%[flags][width][.precision][length]specifier
/*
flags: '-', '+', ' ', '#', '0'
width: number, *
.precision: .number, .*
length: h, l, L

*/

oe_format_t *custom_formats = NULL;
int custom_formats_len = 0;

/*
format starting in N are not allowed 
raison:
    une discusion discord
*/
void oe_register_format(oe_format_t fm) {
    if (fm.specifier[0] == 'N') {
        return;
    }
    custom_formats_len++;
    custom_formats = realloc(custom_formats, sizeof(oe_format_t) * (custom_formats_len));
    custom_formats[custom_formats_len - 1] = fm;
}

int does_is_char_good_format(char c) {
    if (c <= 'Z' && 'A' <= c)
        return 1;
    if (c <= 'z' && 'a' <= c)
        return 1;
    return 0;
}

static int do_format(FILE *fd, char *format, va_list *args, oe_format_arg fm_arg, int *fm_len) {
    int end = 0;
    while (does_is_char_good_format(format[end])) end++;

    char *to_search = malloc(sizeof(char) * (end + 1));
    memcpy(to_search, format, end);
    to_search[end] = '\0';

    *fm_len = end;
    //find blt format
    for(int i = 0; i < blt_formats_len; i++) {
        if (!strcmp(to_search, blt_formats[i].specifier))
            return blt_formats[i].func(fd, args, fm_arg);
    }

    for(int i = 0; i < custom_formats_len; i++) {
        if (!strcmp(to_search, custom_formats[i].specifier))
            return custom_formats[i].func(fd, args, fm_arg);
    }
    return 0;
}

static oe_format_flag parse_flag_fm(char **format) {
    oe_format_flag flag = {0};
    while (1) {
        char current = **format;
        if (current != '-' && current != '+' && current != ' ' && current != '0' && current != '\'' && current != '#' )
            break;
        else {
            switch (current) {
                case '#':
                    flag.hash = 1;
                    break;
                case '0':
                    flag.zero = 1;
                    break;
                case '-':
                    flag.minus = 1;
                    break;
                case '+':
                    flag.plus = 1;
                    break;
                case ' ':
                    flag.space = 1;
                    break;
            }
            (*format)++;
        }
    }
    return flag;
}

int fprintf(FILE *fd, char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = vfprintf(fd, format, args);
    va_end(args);
    return res;
}

int printf(char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = vfprintf(stdout, format, args);
    va_end(args);
    return res;
}

int vfprintf(FILE *fd, char *format, va_list args) {
    int res = 0;

    while (*format) {
        if (*format != '%') {
            fputc(*format, fd);
            res++;
            format++;
        }
        else if (*(format + 1) == '%') {
            fputc('%', fd);
            format++;
            format++;
            res++;
        }
        else {
            //parse format
            oe_format_flag flag;
            i32 width = 0;
            i32 precision = 0;
            char *length = NULL;

            format++;
            flag = parse_flag_fm(&format);

            if (*format == '*') {
                width = -1;
                format++;
            }
            elif (*format <= 9 && 0 <= *format) {
                while (0 <= *format && *format <= 9) {
                    width *= 10;
                    width += (*(format++)) - '0';
                }
            }
            if (*format == '.') {
                if (*format == '*') {
                precision = -1;
                format++;
                }
                elif (*format <= 9 && 0 <= *format) {
                    while (0 <= *format && *format <= 9) {
                        precision *= 10;
                        precision += (*(format++)) - '0';
                    }
                }
            }

            if (*format == 'h') {
                length = "h";
                format++;
            }
            elif (*format == 'l') {
                length = "l";
                format++;
            }
            elif (*format == 'L') {
                length = "L";
                format++;
            }
            int fm_len;
            res += do_format(
                fd,
                format,
                &args,
                (oe_format_arg) {
                    .current_count = res,
                    .flag = flag,
                    .length = length,
                    .precision = precision,
                    .width = width
                },
                &fm_len
            );
            format = &(format[fm_len]);
        }
    }
    return res;
}
