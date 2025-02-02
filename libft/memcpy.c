
#include <kernel.h>
#include <oeuf.h>

void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void	*memmove(void *dest, const void *src, size_t n)
{
	size_t		p;

	if (dest == NULL && src == NULL)
		return (NULL);
	p = 0;
	if (src < dest)
	{
		while (p < n)
		{
			((char *)dest)[n - p - 1] = ((const char *)src)[n - p - 1];
			p++;
		}
	}
	else
	{
		while (p < n)
		{
			((char *)dest)[p] = ((const char *)src)[p];
			p++;
		}
	}
	return (dest);
}

void memset(void *dst, int c, size_t len) {
	for (size_t i = 0; i < len; i++) {
		((u8 *)dst)[i] = c;
	}
}
