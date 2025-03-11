#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>
#include <stddef.h>

////#include "../leak_detector_c.h"

struct list;
typedef struct list list_t;

typedef int (*list_eq)(const void *a, const void *b);
typedef void *(*list_copy_cb)(void *);
typedef void (*list_free_cb)(void *);

typedef struct {
    list_eq      leq;
    list_copy_cb lcopy;
    list_free_cb lfree;
} list_cbs_t;

typedef enum {
    LIST_NONE = 0,
    LIST_SORT = 1 << 0
} list_flags_t;

list_t *list_create(const list_cbs_t *cbs, list_flags_t flags);
void list_destroy(list_t *l);

size_t list_len(list_t *l);

bool list_append(list_t *l, void *v);
bool list_insert(list_t *l, void *v, size_t idx);
bool list_remove(list_t *l, size_t idx);

bool list_index_of(list_t *l, void *v, size_t *idx);
void *list_get(list_t *l, size_t idx);
void *list_take(list_t *l, size_t idx);

void list_start_bulk_add(list_t *l);
void list_end_bulk_add(list_t *l);

bool list_sort(list_t *l, list_eq e);

#endif /* __LIST_H__ */






