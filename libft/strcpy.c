#include "libft.h"

char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = 0;
    return ret;
}

char *strncpy(char *dest, const char *src, size_t n) {
    char *ret = dest;
    while (*src && n) {
        *dest = *src;
        dest++;
        src++;
        n--;
    }
    *dest = 0;
    return ret;
}

char	*strcat(char *dest, char *src)
{
	int	end;
	int	i;

	end = 0;
	while (dest[end] != '\0')
		end++;
	i = 0;
	while (src[i] != '\0')
	{
		dest[end + i] = src[i];
		i++;
	}
	dest[end + i] = '\0';
	return (dest);
}
