#include "libft.h"
#include "oeuf.h"

typedef struct oe_node_t{
    char *key; // if key is NULL node is not set / empty
    void *data;
    struct oe_node_t *next; //last node is NULL
} oe_node_t;


typedef struct {
    oe_node_t *values;
    oe_arch_uint len;
} oe_hash_map_t;

oe_arch_uint oe_hash(char *str) {
    if (str == NULL)
        return 0;

    oe_arch_uint res = 0;

    str--;
    while (*(++str))
        res += ((*str) << 5) + *str;

    return res;
}

oe_hash_map_t oe_new_hash_map(oe_arch_uint len) {
    oe_hash_map_t map = {0};
    map.len = len;
    map.values = malloc(len * sizeof(oe_node_t));

    for (oe_arch_uint i = 0; i < len; i++)
        map.values[i].key = NULL;
    return map;
}

void *oe_hash_map_get(oe_hash_map_t map, char *key, oe_arch_uint hash) {
    oe_node_t *current_node = &(map.values[hash % map.len]);

    while (current_node != NULL) {
        if (!strcmp(current_node->key, key))
            return current_node->data;
        current_node = current_node->next;
    }

    return NULL;
}

oe_node_t *oe_hash_map_get_node(oe_hash_map_t map, char *key, oe_arch_uint hash) {
    oe_node_t *current_node = &(map.values[hash % map.len]);

    while (current_node != NULL) {
        if (!strcmp(current_node->key, key))
            return current_node;
        current_node = current_node->next;
    }

    return NULL;
}

void *oe_hash_map_set(oe_hash_map_t map, char *key, oe_arch_uint hash, void *data) {
    oe_node_t *current_node = &(map.values[hash % map.len]);

    while (current_node != NULL) {
        if (!strcmp(current_node->key, key)) {
            void *old_data = current_node->data;
            current_node->data = data;
            return old_data;
        }
        current_node = current_node->next;
    }
    return NULL;
}

void oe_hash_map_filter(
    oe_hash_map_t map,
    int (*does_keep) (
            const char *key,
            oe_arch_uint hash,
            void *data,
            const oe_hash_map_t map
    ),
    void (*free_data) (
            const char *key,
            oe_arch_uint hash,
            void *data,
            const oe_hash_map_t map
    )) {
    for (oe_arch_uint current_hash = 0; current_hash < map.len; current_hash++) {
        oe_node_t *previous = NULL;
        oe_node_t *node = &(map.values[current_hash]);
        while (node != NULL) {
            if (!(*does_keep)(node->key, current_hash, node->data, map)) {
                (*free_data)(node->key, current_hash, node->data, map);
                oe_node_t *next = node->next;
                free(node);
                free(node->key);
                previous->next = next;
                node = next;
                continue;
            }
            previous = node;
            node = node->next;
        }
    }
}