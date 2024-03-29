#include "test.h"

void assert_equal_buffer(string_t name, Buffer exepted, Buffer got)
{
    if (got == NULL)
    {
        if (exepted == NULL)
            return;

        putchar('X');
        putchar('\n');
        errx(1, "ASSERT ERROR: %s buffer null.", name);
    }

    putchar('.');

    string_t str;

    asprintf(&str, "%s: size", name);
    // printf("string %s\n", str);
    assert_equal_ul(str, exepted->size, got->size);
    free(str);

    asprintf(&str, "%s: data", name);
    assert_equal_arr(str, exepted->data, got->data, got->size);
    free(str);
}
