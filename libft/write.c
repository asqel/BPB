#include <kernel.h>

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
	(void)nmemb;
	(void)stream;
    char *string = (char *)ptr;
    for (size_t i = 0; string[i] != 0 && i < size; i++) {
        screen_add_char(string[i], 0x0f);
    }
}

int fputc(int c, FILE *stream) {
	(void)stream;
	screen_add_char(c, 0x0f);
}
