#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct {
    void **items;
    size_t capacity;
    size_t count;
} list_t;

list_t list_create(int capacity);
void list_append(list_t *l, void *item);
void *list_get(list_t *l, int index);
void list_free(list_t *l);

#endif
