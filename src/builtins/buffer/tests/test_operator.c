#include "./test_operator.h"

Buffer buffer_size_t(size_t value)
{
    Buffer new_buffer;
    if (buffer_constructor_array(&new_buffer, sizeof(size_t), (u_char *)&value) != SUCCESS)
        errx(2, "Error durring buffer creation");
    return new_buffer;
}

void hex_test(Buffer buffer, int error, char *expected)
{
    char *str;
    size_t len;
    int r_error = buffer_to_hex(buffer, &str, &len);

    assert_equal_i("error code", error, r_error);

    if (error == SUCCESS)
    {
        assert_equal_s("value error", expected, str);
        assert_equal_ul("lenght error", len, strlen(str));

        free(str);
    }
    else
        assert_equal_p("to hex error null pointer", NULL, str);

    buffer_destructor(&buffer);
    putchar('|');
}

void to_hex_test()
{
    hex_test(NULL, NO_SELF, NULL);

    Buffer buffer = malloc(sizeof(struct s_buffer));
    buffer->size = SIZE_MAX;
    buffer->data = NULL;
    hex_test(buffer, NO_SPACE, NULL);

    buffer_constructor_const(&buffer, 1, 0xfe);
    hex_test(buffer, SUCCESS, "fe");

    hex_test(buffer_size_t(0), SUCCESS, "0000000000000000");
    hex_test(buffer_size_t(0xf1), SUCCESS, "00000000000000f1");
    hex_test(buffer_size_t(0xcd65137ece71dfaa), SUCCESS, "cd65137ece71dfaa");

    // !TODO - add more tests with diferent size
}

void set_test()
{
    // TODO
}

void get_test()
{
    // TODO
}

Test operator_tests[] = {
    {"buffer_set_index", set_test},
    {"buffer_get_index", get_test},
    {"buffer_to_hex", to_hex_test}};

void test_operator()
{
    run_tests(operator_tests, sizeof(operator_tests) / sizeof(Test), 2);
}
