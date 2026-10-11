#include <stdlib.h>
#include <string.h>

#include "list.h"

////#include "../leak_detector_c.h"


static const size_t list_block_size = 32;

struct list {
    void         **elements;
    size_t          alloced;
    size_t          len;
    list_cbs_t      cbs;
    list_flags_t    flags;
    bool            inbulk;
};


static int list_passthrough_eq(const void *a, const void *b)
{
    void *sa = *(void **)a;
    void *sb = *(void **)b;

    return (int)sa - (int)sb;
}

void merge_sort(void *base, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
    char   *left;
    char   *right;
    size_t  ls;
    size_t  rs;
    size_t  mid;
    size_t  i;
    size_t  j;
    size_t  k;

    if (base == NULL || nel < 2 || width == 0 || cmp == NULL)
        return;

    /* Find the mid and deal with an odd number of elements. */
    mid = nel/2;
    ls  = mid;
    rs  = mid;

    if (nel > 2 && nel % 2 != 0)
        ls++;

    /* Copy the elements into tmp arrays. */
    left  = malloc(ls*width);
    right = malloc(rs*width);
    memcpy(left, base, ls*width);
    memcpy(right, (char*)base+(ls*width), rs*width);

    merge_sort(left, ls, width, cmp);
    merge_sort(right, rs, width, cmp);

    i = 0;
    j = 0;
    k = 0;
    /* Merge the tmp arrays back into the base array in
     * sorted order. */
    while (i < ls && j < rs) {
        if (cmp(left+(i*width), right+(j*width)) <= 0) {
            memcpy((char*)base+(k*width), left+(i*width), width);
            i++;
        } else {
            memcpy((char*)base+(k*width), right+(j*width), width);
            j++;
        }
        k++;
    }

    /* If left is longer than right copy the remaining elements
     * over */
    while (i < ls) {
        memcpy((char*)base+(k*width), left+(i*width), width);
        i++;
        k++;
    }

    /* If right is longer than right copy the remaining elements
     * over */
    while (j < rs) {
        memcpy((char*)base+(k*width), right+(j*width), width);
        j++;
        k++;
    }

    free(right);
    free(left);
}

void *binary_search(const void *base, size_t nel, size_t width, const void *key, int (*cmp)(const void *, const void *))
{
    size_t mid = 0;
    size_t left;
    size_t right;
    int    eq  = -1;

    left  = 0;
    right = nel;

    if (base == NULL || nel == 0 || key == NULL || cmp == NULL)
        return NULL;

    while (left < right) {
        mid = (left + right) / 2;
        eq  = cmp(&key, (char*)base+(mid*width));
        if (eq < 0) {
            right = mid;
        } else if (eq > 0) {
            left = mid+1;
        } else {
            break;
        }
    }

    if (eq != 0)
        return NULL;

    /* Return the first matching element. */
    while (mid > 0 && mid >= left) {
        eq = cmp(&key, (char*)base+((mid-1)*width));
        if (eq != 0) {
            break;
        }
        mid--;
    }

    return (char*)base+(mid*width);
}

size_t binary_insert(const void *base, size_t nel, size_t width, const void *key, int (*cmp)(const void *, const void *))
{
    size_t mid = 0;
    size_t left;
    size_t right;
    int    eq = 0;

    if (base == NULL || nel == 0 || key == NULL || cmp == NULL)
        return 0;

    left  = 0;
    right = nel;

    while (left < right) {
        mid = (left + right) / 2;
        eq  = cmp(&key, (char*)base+(mid*width));
        if (eq < 0) {
            right = mid;
        } else if (eq > 0) {
            left = mid+1;
        } else {
            break;
        }
    }

    /* Insert after the index. */
    if (eq > 0)
        mid++;

    /* Make the insert stable. */
    while (mid < right && eq == 0) {
        mid++;
        eq = cmp(&key, (char*)base+(mid*width));
    }

    return mid;
}
bool binary_search_insert_int(const void *base, size_t nel, size_t width, const void *key, size_t *idx, bool is_insert, int (*cmp)(const void *, const void *))
{
    size_t mid = 0;
    size_t left;
    size_t right;
    int    eq  = -1;

    if (base == NULL || nel == 0 || key == NULL || idx == NULL || cmp == NULL)
        return false;

    left  = 0;
    right = nel;

    while (left < right) {
        mid = (left + right) / 2;
        eq  = cmp(&key, (char*)base+(mid*width));
        if (eq < 0) {
            right = mid;
        } else if (eq > 0) {
            left = mid+1;
        } else {
            break;
        }
    }

    if (is_insert) {
        /* Insert after the index. */
        if (eq > 0) {
            mid++;
        }

        /* Make the insert stable. */
        while (mid < right && eq == 0) {
            mid++;
            eq = cmp(&key, (char*)base+(mid*width));
        }
    } else {
        if (eq != 0) {
            return false;
        }

        /* Return the first matching element. */
        while (mid > 0 && mid >= left) {
            eq = cmp(&key, (char*)base+((mid-1)*width));
            if (eq != 0) {
                break;
            }
            mid--;
        }
    }

    *idx = mid;
    return true;
}


/*
void *binary_search(const void *base, size_t nel, size_t width, const void *key, size_t *idx, int (*cmp)(const void *, const void *))
{
    size_t  myidx;

    if (idx == NULL)
        idx = &myidx;

    if (!binary_search_insert_int(base, nel, width, key, idx, false, cmp))
        return NULL;
    return (void *)base+(*idx*width);
}

size_t binary_insert(const void *base, size_t nel, size_t width, const void *key, int (*cmp)(const void *, const void *))
{
    size_t idx = 0;

    if (!binary_search_insert_int(base, nel, width, key, &idx, true, cmp))
        return 0;
    return idx;
}

*/

list_t *list_create(const list_cbs_t *cbs, list_flags_t flags)
{
    list_t *l;

    l           = malloc(sizeof(*l));
    l->elements = malloc(sizeof(*l->elements)*list_block_size);
    l->alloced  = list_block_size;
    l->len      = 0;

    memset(&(l->cbs), 0, sizeof(l->cbs));
    if (cbs != NULL) {
        l->cbs.leq   = cbs->leq;
        l->cbs.lcopy = cbs->lcopy;
        l->cbs.lfree = cbs->lfree;
    }
    if (l->cbs.leq == NULL)
        l->cbs.leq = list_passthrough_eq;

    l->flags = flags;

    return l;
}


void list_destroy(list_t *l)
{
    size_t i;

    if (l == NULL)
        return;

    if (l->cbs.lfree != NULL) {
        for (i=0; i<l->len; i++) {
            l->cbs.lfree(l->elements[i]);
        }
    }

    free(l->elements);
    free(l);
}


size_t list_len(list_t *l)
{
    if (l == NULL)
        return 0;
    return l->len;
}


bool list_insert(list_t *l, void *v, size_t idx)
{
    if (l == NULL || v == NULL)
        return false;

    if (l->alloced == l->len) {
        l->alloced += list_block_size;
        l->elements = realloc(l->elements, sizeof(*l->elements)*l->alloced);
    }

    if (l->cbs.lcopy != NULL)
        v = l->cbs.lcopy(v);

    if (l->flags & LIST_SORT && !l->inbulk)
        //binary_searchidx(l->elements, l->len, sizeof(*l->elements), v, &idx, true, l->leq);
         binary_search(l->elements, l->len, sizeof(*l->elements), v, l->cbs.leq);

    if (idx > l->len) {
        idx = l->len;
    } else if (idx < l->len) {
        memmove(l->elements+(idx+1), l->elements+idx, (l->len-idx)*sizeof(*l->elements));
    }
    l->elements[idx] = v;

    l->len++;
    return true;
}

bool list_append(list_t *l, void *v)
{
    if (l == NULL || v == NULL)
        return false;
    return list_insert(l, v, l->len);
}

 static bool list_remove_int(list_t *l, size_t idx, bool do_free)
{
    if (l == NULL || idx >= l->len)
        return false;

    if (do_free && l->cbs.lfree != NULL)
        l->cbs.lfree(l->elements[idx]);

    l->len--;
    if (idx == l->len)
        return true;

    memmove(l->elements+idx, l->elements+(idx+1), (l->len-idx)*sizeof(*l->elements));
    return true;
}


   bool list_remove(list_t *l, size_t idx)
{
    return list_remove_int(l, idx, true);
}


bool list_index_of(list_t *l, void *v, size_t *idx)
{
    size_t i;
    bool   found = false;

    if (l == NULL || v == NULL)
        return 0;

    if (l->flags & LIST_SORT && !l->inbulk) {
        if (binary_search(l->elements, l->len, sizeof(*l->elements), v, l->cbs.leq) != NULL) {
            return true;
        }
        return false;
    }

    for (i=0; i<l->len; i++) {
        if (l->cbs.leq(&v, &l->elements[i]) == 0) {
            found = true;
            break;
        }
    }

    if (found && idx != NULL)
        *idx = i;
    return found;
}


void *list_get(list_t *l, size_t idx)
{
    if (l == NULL || idx >= l->len)
        return NULL;
    return l->elements[idx];
}


void *list_take(list_t *l, size_t idx)
{
    void *out;

    if (l == NULL || idx >= l->len)
        return NULL;

    out = list_get(l, idx);
    list_remove_int(l, idx, false);
    return out;
}


void list_start_bulk_add(list_t *l)
{
    if (l == NULL)
        return;
    l->inbulk = true;
}

void list_end_bulk_add(list_t *l)
{
    if (l == NULL)
        return;
    l->inbulk = false;
    list_sort(l, NULL);
}


bool list_sort(list_t *l, list_eq e)
{
    if (l == NULL)
        return false;

    if (l->flags & LIST_SORT) {
        if (e != NULL) {
            l->cbs.leq = e;
        } else {
            e = l->cbs.leq;
        }
    }

    if (e == NULL)
        return false;

    merge_sort(l->elements, l->len, sizeof(*l->elements), l->cbs.leq);
}


