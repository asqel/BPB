#include <kernel.h>
#include <oeuf.h>
#include <libft.h>
#include <fal.h>

/*

file systeme for bpb (faloc (fat32 alloc lol))

*/
fal_element_t fal_root = {0};

void fal_add_element(fal_dir_t *dir, fal_element_t *element) {
	dir->element_count++;
	if (dir->element_count >= dir->element_alloc_len) {
		dir->element_alloc_len = dir->element_alloc_len + 10;
		dir->elements = realloc(dir->elements, dir->element_alloc_len * sizeof(fal_element_t));
	}
	dir->elements[dir->element_count - 1] = *element;
}

void fal_add_file(fal_dir_t *dir, char *name, u8 *data, u32 size) {
	fal_element_t element = {0};
	element.type = FAL_T_FILE;
	element.name = strdup(name);
	element.value.file.data = malloc(size);
	memcpy(element.value.file.data, data, size);
	element.value.file.size = size;
	element.value.file.data_alloc_len = size;
	fal_add_element(dir, &element);
}

void fal_add_dir(fal_dir_t *dir, char *name) {
	fal_element_t element = {0};
	element.type = FAL_T_DIR;
	element.name = strdup(name);
	element.value.dir.element_count = 0;
	element.value.dir.elements = NULL;
	element.value.dir.element_alloc_len = 0;
	element.value.dir.parent = dir;
	fal_add_element(dir, &element);
}

fal_dir_t *fal_get_dir_from(fal_dir_t *dir, char *name) {
	for (u32 i = 0; i < dir->element_count; i++) {
		if (dir->elements[i].type == FAL_T_DIR && strcmp(dir->elements[i].name, name) == 0) {
			return &dir->elements[i].value.dir;
		}
	}
	return NULL;
}

void fal_add_link(fal_dir_t *dir, char *name, char *link) {
	fal_element_t element = {0};
	element.type = FAL_T_LINK;
	element.name = strdup(name);
	element.value.link.link = strdup(link);
	element.value.link.type = link[0] == '/' ? FAL_LINK_T_ABS : FAL_LINK_T_RELATIVE;
	fal_add_element(dir, &element);
}

void add_element_from_disk(fal_dir_t *dir, u8 *disk, u32 disk_size) {
	u32 i = 0;

	while (i < disk_size) {
		u8 type = disk[i];
		if (type == FAL_T_LINK) {
			char *name = &disk[i + 1];
			u32 name_len = strlen(name);
			char *link = &disk[i + 1 + name_len + 1];
			fal_add_link(dir, name, link);
			i += 1 + name_len + 1 + strlen(link) + 1;
			continue;
		}
		char *name = &disk[i + 1];
		u32 name_len = strlen(name);
		u32 content_len = *(u32 *)(&disk[i + 1 + name_len + 1]);
		u8 *content = &disk[i + 1 + name_len + 1 + 4];
		switch (type) {
			case FAL_T_FILE:
				u8 *new_content = malloc(content_len);
				memcpy(new_content, content, content_len);
				fal_add_file(dir, name, new_content, content_len);
				break;
			case FAL_T_DIR:
				fal_add_dir(dir, name);
				add_element_from_disk(fal_get_dir_from(dir, name), content, content_len);
				break;
			case FAL_T_SPECIAL_FILE:
				break;
		}
		i += 1 + name_len + 1 + 4 + content_len;
	}
}

void fal_init(u8 *disk, u32 disk_size) {
	fal_root.type = FAL_T_DIR;
	fal_root.name = strdup("/");
	fal_root.value.dir.element_count = 0;
	fal_root.value.dir.elements = NULL;
	fal_root.value.dir.element_alloc_len = 0;
	fal_root.value.dir.parent = &fal_root;

	add_element_from_disk(&fal_root.value.dir, disk, disk_size);
}

// if len is -1 use strcmp else use strncmp
fal_element_t *fal_get_element_from(fal_dir_t *dir, char *name, int len) {
	if (len == -1)
		len = strlen(name);
	for (u32 i = 0; i < dir->element_count; i++) {
		if (strlen(dir->elements[i].name) != len)
			continue;
		if (!strncmp(dir->elements[i].name, name, len))
			return &(dir->elements[i]);
	}
	return NULL;
}

static char *fal_path_link_relative(char *abs_path, int i, char *link, char *lname) {
	u32 len1 = i;
	u32 len2 = strlen(&(abs_path[i + strlen(lname)]));
	char *res = malloc(sizeof(char) * (len1 + 1 + strlen(link) + 1 + len2 + 1));
	if (res == NULL)
		return NULL;
	res[0] = '\0';
	strncat(res, abs_path, len1);
	strcat(res, "/");
	strcat(res, link);
	strcat(res, "/");
	strcat(res, &(abs_path[i + strlen(lname)]));
	return res;

}


//takes only absolute path (may contain ./ ../ or link)
fal_element_t *fal_get_element(char *abs_path) {
	if (abs_path == NULL)
		return &fal_root;
	if (abs_path[0] != '/')
		return NULL;
	fal_element_t *current = &fal_root;
	current->parent = &fal_root;
	u32 i = 1;
	while (abs_path[i] != '\0') {

		if (abs_path[i] == '/') {
			i++;
			continue;
		}
		if (current->type != FAL_T_DIR && current->type != FAL_T_LINK)
			return NULL;
		u32 end = i;
		while (abs_path[end] != '/' && abs_path[end] != '\0')
			end++;
		u32 len = end - i;
		if (len == 1 && abs_path[i] == '.') {
			i = end;
			continue;
		}
		if (len == 2 && abs_path[i] == '.' && abs_path[i + 1] == '.') {
			i = end;
			current = current->parent;
			continue;
		}
		fal_element_t *element = fal_get_element_from(&current->value.dir, &abs_path[i], len);
		if (element == NULL)
			return NULL;
		if (element->type == FAL_T_LINK) {
			if (element->value.link.type == FAL_LINK_T_ABS) {
				char *new_path = oe_str_cat_new3(element->value.link.link, "/", &abs_path[end]);
				fal_element_t *res = fal_get_element(new_path);
				free(new_path);
				return res;
			}
			else {
				char *new_path = fal_path_link_relative(abs_path, i, element->value.link.link, element->name);
				if (new_path == NULL)
					return NULL;
				fal_element_t *res = fal_get_element(new_path);
				free(res);
				return res;
			}
		}
		element->parent = current;
		current = element;
		i = end;
	}
	return current;
}


fal_dir_t *fal_get_dir(char *abs_path) {
	fal_element_t *element = fal_get_element(abs_path);
	if (element == NULL || element->type != FAL_T_DIR)
		return NULL;
	return &element->value.dir;
}

fal_file_t *fal_get_file(char *abs_path) {
	fal_element_t *element = fal_get_element(abs_path);
	if (element == NULL || element->type != FAL_T_FILE)
		return NULL;
	return &element->value.file;
}

void fal_print_tree(fal_element_t *dir, int indent, FILE *file) {
	for (int i = 0; i < indent - 2; i++)
		fprintf(file, " ");
	fprintf(file, "%s", dir->name);
	if (strncmp(dir->name, "/", 1))
		fprintf(file, "/");
	fprintf(file, "\n");
	for (u32 i = 0; i < dir->value.dir.element_count; i++) {
		if (dir->value.dir.elements[i].type == FAL_T_DIR) {
			fal_print_tree(&dir->value.dir.elements[i], indent + 2, file);
		}
		if (dir->value.dir.elements[i].type == FAL_T_FILE) {
			for (int j = 0; j < indent; j++)
				fprintf(file, " ");
			fprintf(file, "%s addr %p\n", dir->value.dir.elements[i].name, (u32)&(dir->value.dir.elements[i]));
		}
		if (dir->value.dir.elements[i].type == FAL_T_LINK) {
			for (int j = 0; j < indent; j++)
				fprintf(file, " ");
			fprintf(file, "%s@\n", dir->value.dir.elements[i].name);
		}
		if (dir->value.dir.elements[i].type == FAL_T_SPECIAL_FILE) {
			for (int j = 0; j < indent; j++)
				fprintf(file, " ");
			fprintf(file, "%s*\n", dir->value.dir.elements[i].name);
		}
	}

}

void fal_fprint_element(FILE *fd, fal_element_t *element) {
	if (element == NULL)
		fprintf(fd, "FAL{NULL}");
	else
		fprintf(fd, "FAL{type: %d, name: %s}\n", element->type, element->name);
}