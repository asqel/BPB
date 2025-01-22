#include <kernel.h>
#include <oeuf.h>
#include <libft.h>

char *oe_str_cat_new3(const char *str1, const char *str2, const char *str3) {
	u32 lens[3] = {strlen(str1), strlen(str2), strlen(str3)};

	char *res = malloc(sizeof(char) * (lens[0] + lens[1] + lens[2] + 1));
	if (res == NULL)
		return NULL;
	for (u32 i = 0; i < lens[0]; i++)
		res[i] = str1[i];
	for (u32 i = 0; i < lens[1]; i++)
		res[i + lens[0]] = str2[i];
	for (u32 i = 0; i < lens[2]; i++)
		res[i + lens[0] + lens[1]] = str3[i];
	res[lens[0] + lens[1] + lens[2]] = '\0';
	return res;
}