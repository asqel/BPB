#include <kernel.h>

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
	(void)nmemb;
	(void)stream;
    char *string = (char *)ptr;
    for (size_t i = 0; string[i] != 0 && i < size; i++) {
        screen_add_char(string[i], 0x0f);
    }
    return size;
}

int fputc(int c, FILE *stream) {
	(void)stream;
	screen_add_char(c, 0x0f);
    return c;
}

int fputs(const char *s, FILE *stream) {
    (void)stream;
    for (size_t i = 0; s[i] != 0; i++) {
        screen_add_char(s[i], 0x0f);
    }
    return 0;
}

int puts(const char *s) {
    for (size_t i = 0; s[i] != 0; i++) {
        screen_add_char(s[i], 0x0f);
    }
    screen_add_char('\n', 0x0f);
    return 0;
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