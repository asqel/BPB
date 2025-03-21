#include "oeuf.h"
#include <libft.h>

size_t alloc_count = 0;

u8 *heap_info = 0; //1 == ocupied ; 2 == start of block (2bit each)
u8 *heap = 0;

#define is_occupied_or_start(x) (heap_info[(size_t)((x) - heap) / 4] & ((u8)0b11 << (2 * ((size_t)((x) - heap) % 4))))
#define is_occupied(x) (heap_info[(size_t)((x) - heap) / 4] & ((u8)0b01 << (2 * ((size_t)((x) - heap) % 4))))
#define is_start(x) (heap_info[(size_t)((x) - heap) / 4] & ((u8)0b10 << (2 * ((size_t)((x) - heap) % 4))))
#define set_start(x) (heap_info[(size_t)((x) - heap) / 4] |= ((u8)0b10 << (2 * ((size_t)((x) - heap) % 4))))
#define set_occupied(x) (heap_info[(size_t)((x) - heap) / 4] |= ((u8)0b01 << (2 * ((size_t)((x) - heap) % 4))))
#define clear_info(x) (heap_info[(size_t)((x) - heap) / 4] &= ~((u8)0b11 << (2 * ((size_t)((x) - heap) % 4))))

void heap_init(u8 *heap_start, u32 heap_size) {
	heap = heap_start;
	heap_info = heap_start + heap_size;
	for (int i = 0; i < HEAP_SIZE / 4; i++)
		heap_info[i] = 0;
}

u8 *heap_init_with_disk(u8 *heap_start, u32 heap_size, u8 *disk, u32 disk_size) {
	heap = heap_start;
	memcpy(heap, disk, disk_size);
	heap_info = heap_start + heap_size;
	for (int i = 0; i < HEAP_SIZE / 4; i++)
		heap_info[i] = 0;
	return malloc(disk_size);
}

void *malloc(size_t size) {
	if (size == 0)
		return NULL;

	for (size_t i = 0; i < HEAP_SIZE - size + 1; i++) {
		u8 is_here = 1;
		for (size_t k = 0; k < size; k++) {
			if (is_occupied_or_start(&heap[i + k])) {
				is_here = 0;
				break;
			}
		}
		if (is_here) {
			set_start(&heap[i]);
			for (size_t k = 1; k < size; k++) {
				set_occupied(&heap[i + k]);
			}
			alloc_count++;
			return &heap[i];
		}
	}
    puts("malloc: out of memory");
	return NULL;
}

void free(void *ptr) {
    u8 *ptr2 = (u8 *)ptr;
    if (ptr == NULL)
        return ;
    if ((ptr2 < heap || ptr2 >= heap + HEAP_SIZE) || !is_start(ptr2)) {
        puts("free: invalid pointer");
		return ;
	}
	int i = 0;
	while (i == 0 || is_occupied(ptr2 + i)) {
		clear_info(ptr2 + i);
		i++;
	}
	alloc_count--;
}

size_t get_alloc_size(void *ptr) {
	u8 *ptr2 = (u8 *)ptr;
	size_t len = 0;

	if ((ptr2 < heap || ptr2 >= heap + HEAP_SIZE) || !is_start(ptr2))
		return 0;
	len++;
	while (is_occupied(ptr2 + len))
		len++;
	return len;

}

inline static size_t min(size_t a, size_t b) {
	if (a < b)
		return a;
	return b;
}
void *realloc(void *ptr, size_t size) {
	// int alloc_len = get_alloc_size(ptr);
	// if (alloc_len == -1) {
		// return NULL;
	// }
	// if (size == 0) {
		// free(ptr);
		// return NULL;
	// }
	// if (alloc_len == size)
		// return ptr;
	// if (size < alloc_len) {
		// for (int i = size; i < alloc_len; i++) {
			// clear_info((u8 *)ptr + i);
		// }
		// return ptr;
	// }
	// // check if is there room just after
	// u8 is_there_room = 1;
	// for (int i = alloc_len; i < size; i++) {
		// if (is_occupied_or_start((u8 *)ptr + i)) {
			// is_there_room = 0;
			// break;
		// }
	// }
	// if (is_there_room) {
		// for (int i = alloc_len; i < size; i++) {
			// set_occupied((u8 *)ptr + i);
		// }
		// return ptr;
	// }
	// void *new_ptr = malloc(size);
	// if (!new_ptr) {
		// free(ptr);
		// return NULL;
	// }
	// memcpy(new_ptr, ptr, alloc_len);
	// free(ptr);
	// return new_ptr;

	if (size == 0) {
		free(ptr);
		return NULL;
	}

	void *new_ptr = malloc(size);
	if (!new_ptr)
		return NULL;
	memcpy(new_ptr, ptr, min(get_alloc_size(ptr), size));
	free(ptr);
	return new_ptr;
}
void *calloc(size_t nmemb, size_t size) {
    void *ptr = malloc(nmemb * size);
    if (!ptr)
        return NULL;
    for (size_t i = 0; i < nmemb * size; i++)
        ((u8 *)ptr)[i] = 0;
    return ptr;
}

char *strdup(const char *s) {
    size_t len = strlen(s);
    char *ptr = malloc(len + 1);
    if (!ptr)
        return NULL;
    memcpy(ptr, s, len + 1);
    return ptr;
}

size_t get_alloc_count() {
	return alloc_count;
}