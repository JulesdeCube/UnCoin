#include "test_getter.h"

Buffer buffer;

void get_size_test()
{
    // normal used
    if (buffer_constructor_size(&buffer, 10) != SUCCESS)
        errx(2, "Error durring buffer creation");
    assert_equal_ul("buffer size.", 10, buffer_get_size(buffer));
    buffer_destructor(&buffer);

    // null size
    if (buffer_constructor_size(&buffer, 0) != SUCCESS)
        errx(2, "Error durring buffer creation");
    assert_equal_ul("buffer size.", 0, buffer_get_size(buffer));
    buffer_destructor(&buffer);

    // null buffer
    assert_equal_ul("buffer size.", 0, buffer_get_size(NULL));
}

void get_data_test()
{
    // normal used
    if (buffer_constructor_size(&buffer, 10) != SUCCESS)
        errx(2, "Error durring buffer creation");
    assert_equal_p("buffer data.", buffer->data, buffer_get_data(buffer));
    assert_not_equal_p("buffer data null.", NULL, buffer_get_data(buffer));
    buffer_destructor(&buffer);

    // null size
    if (buffer_constructor_size(&buffer, 0) != SUCCESS)
        errx(2, "Error durring buffer creation");
    assert_equal_p("buffer data.", buffer->data, buffer_get_data(buffer));
    assert_not_equal_p("buffer data null.", NULL, buffer_get_data(buffer));
    buffer_destructor(&buffer);

    // null buffer
    assert_equal_p("buffer data null.", NULL, buffer_get_data(NULL));
}

Test getter_tests[] = {
    {"buffer_get_size", get_size_test},
    {"buffer_get_data", get_data_test}};

void test_getter()
{
    run_tests(getter_tests, sizeof(getter_tests) / sizeof(Test), 2);
}
