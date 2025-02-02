#include <kernel.h>

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    char *string = (char *)ptr;
	int count = 0;
    for (size_t i = 0; i < size * nmemb; i++) {
		fputc(string[i], stream);
		if ((i + 1) % size == 0)
			count++;
    }
    return count;
}

int fputc(int c, FILE *stream) {
	if (stream == stdout)
		screen_add_char(c, 0x0f);
	if (stream == serialout)
		serial_putc(c);
    return c;
}

int fputs(const char *s, FILE *stream) {
	int len = 0;

	while (s[len] != '\0')
		len++;
    return fwrite(s, 1, len, stream);
}

int puts(const char *s) {
    	int len = 0;

	while (s[len] != '\0')
		len++;
    if (fwrite(s, 1, len, stdout) == len)
        return fputc('\n', stdout);
    return -1;
}


int	is_char_space(char c)
{
	if ('\f' == c)
		return (1);
	if ('\n' == c)
		return (1);
	if ('\r' == c)
		return (1);
	if ('\t' == c)
		return (1);
	if ('\v' == c)
		return (1);
	if (' ' == c)
		return (1);
	if ('\t' == c)
		return (1);
	return (0);
}

int	atoi(char *str)
{
	int	res;
	int	sign;

	while (is_char_space(*str))
		str++;
	sign = 0;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = !sign;
		str++;
	}
	res = 0;
	while ('0' <= *str && *str <= '9')
	{
		res *= 10;
		if (sign)
			res -= *str - '0';
		else
			res += *str - '0';
		str++;
	}
	return (res);
}

// #include <stdio.h>

// int	main(int argc, char **argv)
// {
// 	int	i;

// 	i = 0;
// 	while (i < argc)
// 	{
// 		printf("%s : %d\n", argv[i], ft_atoi(argv[i]));
// 		i++;
// 	}
// 	return (0);
// }