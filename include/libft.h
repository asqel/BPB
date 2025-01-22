#ifndef LIBFT_H
#define LIBFT_H

#include <kernel.h>

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
size_t strlen(const char *s);
void	*memmove(void *dest, const void *src, size_t n);
char	*strcat(char *dest, char *src);
char	*strncat(char *dest, const char *src, size_t n);
int	atoi(char *str);

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

#define putchar(c) fputc(c, stdout)

int fputs(const char *s, FILE *stream);
int fputc(int c, FILE *stream);
int puts(const char *s);



#endif
