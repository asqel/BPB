#ifndef OEUF_H
#define OEUF_H
/*
if OEUF64 is defined 64bits functionalities will be enabled
*/

#include <kernel.h>
#include <stdarg.h>

#undef NULL
#define NULL ((void *)0)

typedef struct {
    char minus;
    char plus;
    char space;
    char hash;
    char zero;
} oe_format_flag;

extern oe_format_flag null_flag;

#define FLAG_CMP(X, Y)  (X.minus == Y.minus && X.plus == Y.plus && X.space == Y.space&& X.hash == Y.hash && X.zero == Y.zero &&)

typedef struct {
    oe_format_flag flag;
    i32 width; // if -1 then it's *
    i32 precision; // same
    char *length;
    int current_count;
} oe_format_arg;


typedef struct {
    int (*func)(FILE *, va_list *, oe_format_arg);
    char *specifier;
} oe_format_t;

// read a file at `path` and returns its content as null terminated string
char *oe_read_file(const char *path);

// read a file at `path` and returns its content and put its length  in *len
char *oe_read_file_bin(const char *path, size_t *len);


// return a new string which is the concatenation of `s1` and `s2` as null terminated string 
char *oe_str_cat_new(const char *s1, const char *s2);

// return a new string which is the concatenation of `s1` `s2` and `s3` as null terminated string 
char *oe_str_cat_new3(const char *s1, const char *s2, const char *s3);

// return a new string which is the concatenation of `s1` `s2` `s3`and `s4` as null terminated string 
char *oe_str_cat_new4(const char *s1, const char *s2, const char *s3, const char *s4);

/*
split a string by a separator
strings can be empty but not null
null pointer indicate end
every pointer of result may be free and result may be freed
ex:
    split("abc :! df :!:!uu", ":!")
    -> ["abc", " df ", "", "uu", NULL]
*/
char **uit_str_split(char *str, char* sep);


/*
    apply the selection sort on an array `arr` of integers of length `len`
*/
void oe_selection_sort_i(int *arr, int len);


/*
apply selection sort on arr using comp
comp is a function that takes 2 void * a and b and return a < b
*/
void oe_selection_sort_c(void *arr, int len, size_t elem_size, int (*comp)(void *, void *));



//return the length of a number `n` in base 10
// if the number return len(-n) + 1
// -2**31 will be correctly handled and will not overflow
int oe_number_len(u32 n);


//return the length of a number `n` in base `b`
// if the number is negative the length of its opposite will be returned + 1
// -2**31 will be correctly handled and will not overflow
int oe_number_len_b(u32 n, u32 b);


/*	write to a file at `path` the content of `content` of length `len`
    if an error occured during writting or opening the file 1 will be returned

*/
int oe_write_file(char *path, char *content, int len);


/*	return a copy of `str` where sequence of `from` are replaced by `to`
    ex:
        replace("bonjour salut", "salut", "hi")
    if `from` or `to` is NULL a copy of `str` is returned
    if str is NULL, NULL is retured
*/
char *oe_str_replace(char *str, char *from, char *to);

extern oe_format_t blt_formats[];

extern int blt_formats_len;

extern oe_format_t *custom_formats;
extern int custom_formats_len;

int fprintf(FILE *fd, char *format, ...);
int vfprintf(FILE *fd, char *format, va_list args);

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
char *strdup(const char *s);

#endif
