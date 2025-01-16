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
	fprintf(serialout, "le ptr %x\n", element.value.file.data);
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
				fprintf(serialout, "added file %s %d\n", name, content_len);
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


//takes only absolute path (may contain ./ ../ or link)
fal_element_t *fal_get_element(char *abs_path) {
	if (abs_path == NULL)
		return &fal_root;
	if (abs_path[0] != '/')
		return NULL;
	fal_element_t *current = &fal_root;
	current->parent = &current->value.dir;

	int i = 0;
	while (abs_path[i] == '\0') {
		fprintf(serialout, "le i est %d\n", i);
		if (abs_path[i] == '/') {
			i++;
			continue;
		}
		if (current->type != FAL_T_DIR && current->type != FAL_T_LINK)
			return NULL;
		if (current->type == FAL_T_DIR) {
			int name_len = 0;
			for (int k = i; abs_path[k] != '\0' && abs_path[k] != '/'; k++)
				name_len++;
			if (!strncmp(&abs_path[i], "..", 2)) {
				i += 2;
				current = current->parent;
				continue;
			}
			if (!strncmp(&abs_path[i], ".", 1)) {
				i += 1;
				continue;
			}
			int found = 0;
			for (int k = 0; k < current->value.dir.element_count; k++) {
				if (!strncmp(current->value.dir.elements[k].name, &abs_path[i], name_len)) {
					i += k;
					current->value.dir.elements[k].parent = current;
					current = &current->value.dir.elements[k];
					found  = 1;
					break;
				}
			}
			if (!found)
				return NULL;
			continue;
		}
		if (current->type == FAL_T_LINK) {
			if (current->value.link.type == FAL_LINK_T_ABS) {
				char *new_path = malloc(strlen(current->name) + strlen(&abs_path[i]) + 2);
				new_path[0] = '\0';
				strcat(new_path, current->value.link.link);
				strcat(new_path, "/");
				strcat(new_path, &abs_path[i]);
				fal_element_t *element = fal_get_element(new_path);
				free(new_path);
				return element;
			}
			if (current->value.link.type == FAL_LINK_T_RELATIVE) {
				int link_parent_end = i - 1;
				while ( link_parent_end >= 0 && abs_path[link_parent_end] != '/')
					link_parent_end--;
				char *new_path = malloc(link_parent_end + 1 + strlen(current->value.link.link) + 1);
				new_path[0] = '\0';
				for (int k = 0; k < link_parent_end; k++)
					new_path[k] = abs_path[k];
				new_path[link_parent_end] = '\0';
				strcat(new_path, "/");
				strcat(new_path, current->value.link.link);
				fal_element_t *element = fal_get_element(new_path);
				free(new_path);
				return element;
			}
		}
		return NULL;
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
	for (int i = 0; i < dir->value.dir.element_count; i++) {
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