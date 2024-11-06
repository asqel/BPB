#ifndef LIBFT_H
#define LIBFT_H

#include <kernel.h>

int strcmp(const char *s1, const char *s2);
void *memcpy(void *dest, const void *src, size_t n);

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

int fputc(int c, FILE *stream);

#endif
