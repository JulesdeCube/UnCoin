#include "test.h"
#include "../buffer.h"

void test_destructor()
{
    // null reference
    buffer_destructor(NULL);
    putchar('.');

    // null buffer
    Buffer buffer = NULL;
    buffer_destructor(&buffer);
    putchar('.');

    // normal use
    if (buffer_constructor_const(&buffer, 10, 2) != SUCCESS)
        errx(2, "Error durring buffer creation");

    buffer_destructor(&buffer);
    assert_equal_p("buffer pointer.", NULL, buffer);

    // test when data is null
    if (buffer_constructor_const(&buffer, 10, 2) != SUCCESS)
        errx(2, "Error durring buffer creation");

    free(buffer->data);
    buffer->data = NULL;
    buffer_destructor_safe(&buffer);
    assert_equal_p("buffer pointer.", NULL, buffer);
}