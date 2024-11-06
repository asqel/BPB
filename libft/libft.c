#include <kernel.h>

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while (n && *s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
        n--;
    }
    return *s1 - *s2;
}

size_t strlen(const char *s) {
    size_t len = 0;
    while (*s) {
        len++;
        s++;
    }
    return len;
}
