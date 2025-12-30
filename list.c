#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

list_t list_create(int capacity) {
    if (capacity < 1) { capacity = 1; }

    list_t result;
    result.items = malloc(capacity * sizeof(void *));
    if (result.items == NULL) {
        fprintf(stderr, "list_create: Failed to allocate initial memory");
        abort();
    }

    result.capacity = capacity;
    result.count = 0;
    return result;
}

void list_append(list_t *l, void *item) {
    if (l->count == l->capacity) {
        void **new_items = realloc(l->items, l->capacity * 2 * sizeof(void *));
        if (new_items == NULL) {
            fprintf(stderr, "list_append: Failed to reallocate memory");
            abort();
        }

        l->items = new_items;
        l->capacity *= 2;
    }

    l->items[l->count++] = item;
}

void *list_get(list_t *l, int index) {
    assert(index >= 0 && index < l->count && "Index out of bounds");
    return l->items[index];
}

void list_free(list_t *l) {
    free(l->items);
    l->items = NULL;
    l->count = 0;
    l->capacity = 0;
}
