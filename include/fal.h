#ifndef ARCHI_H
#define ARCHI_H

#include <kernel.h>



enum {
	FAL_T_FILE = 'F',
	FAL_T_DIR = 'D',
	FAL_T_LINK = 'L',
	FAL_T_SPECIAL_FILE = 'S',
};

struct fal_file_t{
	u8 *data;
	u32 size;
	u32 data_alloc_len;
};

struct fal_special_file_t {
	int (*read)(void *data, u32 size, u32 offset);
	int (*write)(void *data, u32 size, u32 offset);
	void *data;
};

enum {
	FAL_LINK_T_ABS,
	FAL_LINK_T_RELATIVE,
};

struct fal_link_t {
	char *link;
	u8 type;
};

struct fal_dir_t {
	u32 element_count;
	struct fal_element_t *elements;
	u32 element_alloc_len;
	struct fal_element_t *parent;
};

struct fal_element_t {
	u8 type;
	char *name;
	union {
		struct fal_file_t file;
		struct fal_dir_t dir;
		struct fal_special_file_t special_file;
		struct fal_link_t link;
	} value;
	struct fal_dir_t *parent; // not guarenteed to be correct (only used by get_element)
};


typedef struct fal_dir_t fal_dir_t;
typedef struct fal_element_t fal_element_t;
typedef struct fal_file_t fal_file_t;
typedef struct fal_special_file_t fal_special_file_t;
typedef struct fal_link_t fal_link_t;

extern fal_element_t fal_root;

fal_dir_t *fal_get_dir(char *abs_path);
fal_file_t *fal_get_file(char *abs_path);
void fal_print_tree(fal_element_t *dir, int indent, FILE *file);
void fal_init(u8 *disk, u32 disk_size);

#endif