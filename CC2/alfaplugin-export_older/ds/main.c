#include <stdio.h>
#include <stdlib.h>
#include "list_str.h"

int main(int argc, char **argv)
{
    const char *s1 = "s1";
    const char *s2 = "s2";
    const char *s3 = "s3";
    char       *out;
    list_str_t *ls;

    printf("create\n");
    ls = list_str_create(LIST_STR_NONE);
    printf("\tlen = %zu\n", list_str_len(ls));

    printf("append s1\n");
    list_str_append(ls, s1);
    printf("\tidx 0 = %s\n", list_str_get(ls, 0));
    printf("\tlen = %zu\n", list_str_len(ls));

    printf("append s2\n");
    list_str_append(ls, s2);
    printf("\tidx 0 = %s\n", list_str_get(ls, 0));
    printf("\tidx 1 = %s\n", list_str_get(ls, 1));
    printf("\tlen = %zu\n", list_str_len(ls));

    printf("insert s3 at 1\n");
    list_str_insert(ls, s3, 1);
    printf("\tidx 0 = %s\n", list_str_get(ls, 0));
    printf("\tidx 1 = %s\n", list_str_get(ls, 1));
    printf("\tidx 2 = %s\n", list_str_get(ls, 2));
    printf("\tlen = %zu\n", list_str_len(ls));

    printf("hash s3? %s\n", list_str_index_of(ls, s3, NULL)?"YES":"NO");

    out = list_str_join(ls, ", ");
    printf("join:\n%s\n", out);
    free(out);

    list_str_destroy(ls);
    return 0;
}
