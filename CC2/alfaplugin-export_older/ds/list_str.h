#ifndef __LIST_STR_H__
#define __LIST_STR_H__

#include <stdbool.h>
#include <stddef.h>

struct list_str;
typedef struct list_str list_str_t;

typedef enum {
    LIST_STR_NONE    = 0,
    LIST_STR_SORT    = 1 << 0,
    LIST_STR_CASECMP = 1 << 1
} list_str_flags_t;

list_str_t *list_str_create(list_str_flags_t flags);
void list_str_destroy(list_str_t *l);

size_t list_str_len(list_str_t *l);

bool list_str_append(list_str_t *l, const char *v);
bool list_str_insert(list_str_t *l, const char *v, size_t idx);
bool list_str_remove(list_str_t *l, size_t idx);

bool list_str_index_of(list_str_t *l, const char *v, size_t *idx);
const char *list_str_get(list_str_t *l, size_t idx);
char *list_str_take(list_str_t *l, size_t idx);

char *list_str_join(list_str_t *l, const char *sep);

#endif /* __LIST_STR_H__ */
