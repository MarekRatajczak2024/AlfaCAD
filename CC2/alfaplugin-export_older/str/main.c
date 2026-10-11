#include <stdio.h>
#include <stdlib.h>
#include "str_builder.h"

int main(int argc, char **argv)
{
    str_builder_t *sb;
    char           *str;

    sb = str_builder_create();
    printf("create\n");
    printf("\tsb len=%zu\n\tsb='%s'\n", str_builder_len(sb), str_builder_peek(sb));

    str_builder_add_str(sb, "Test", 0);
    printf("Add 'Test'\n");
    printf("\tsb len=%zu\n\tsb='%s'\n", str_builder_len(sb), str_builder_peek(sb));

    str_builder_add_str(sb, " ", 0);
    printf("Add ' '\n");
    printf("\tsb len=%zu\n\tsb='%s'\n", str_builder_len(sb), str_builder_peek(sb));

    str_builder_add_int(sb, 123);
    printf("Add int 123\n");
    printf("\tsb len=%zu\n\tsb='%s'\n", str_builder_len(sb), str_builder_peek(sb));

    str_builder_truncate(sb, str_builder_len(sb)-2);
    printf("Truncate -2\n");
    printf("\tsb len=%zu\n\tsb='%s'\n", str_builder_len(sb), str_builder_peek(sb));

    str_builder_drop(sb, 3);
    printf("Drop 3\n");
    printf("\tsb len=%zu\n\tsb='%s'\n", str_builder_len(sb), str_builder_peek(sb));

    str = str_builder_dump(sb, NULL);
    printf("Dump\n");
    printf("\tsb len=%zu\n\tsb='%s'\n", str_builder_len(sb), str_builder_peek(sb));
    printf("\tstr='%s'\n", str);
    free(str);

    str_builder_destroy(sb);
    return 0;
}


