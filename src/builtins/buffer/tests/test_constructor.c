#include "test_constructor.h"

#define size_1 10
#define size_2 0

Buffer buffer;
error_t error;

void buff_des(Buffer buffer)
{
    free(buffer->data);
    free(buffer);
}

void buffer_test(Buffer new_buffer, int new_code, int error, size_t size, u_char *data)
{
    // check the error code
    assert_equal_i("Error code", error, new_code);
    // if it's not a success check if the buffer is correcly set
    if (error != SUCCESS)
        assert_equal_p("Buffer pointer", NULL, new_buffer);
    // if all wen't good do the buffer check
    else
    {
        // test the size
        assert_equal_ul("Size", size, new_buffer->size);
        // if a test arry is provided do the test
        if (data != NULL)
            assert_equal_arr("Data", new_buffer->data, data, size);
        // free the buffer
        buff_des(new_buffer);
    }
    // add a buffer test separator
    putchar('|');
}

void constructor_size_test()
{
    // normal use
    error = buffer_constructor_size(&buffer, 10);
    buffer_test(buffer, error, SUCCESS, 10, NULL);

    // no size
    error = buffer_constructor_size(&buffer, 0);
    buffer_test(buffer, error, SUCCESS, 0, NULL);

    // to big
    error = buffer_constructor_size(&buffer, SIZE_MAX);
    buffer_test(buffer, error, NO_SPACE, SIZE_MAX, NULL);

    // no return pointer
    error = buffer_constructor_size(NULL, 0);
    buffer_test(buffer, error, NO_SELF, 0, NULL);
}

void constructor_const_test()
{
    // normal use
    u_char data_1[size_1] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    error = buffer_constructor_const(&buffer, size_1, 1);
    buffer_test(buffer, error, SUCCESS, size_1, data_1);

    // void buffer
    u_char data_2[size_2] = {};
    error = buffer_constructor_const(&buffer, size_2, 1);
    buffer_test(buffer, error, SUCCESS, size_2, data_2);

    // to big buffer
    error = buffer_constructor_const(&buffer, SIZE_MAX, 2);
    buffer_test(buffer, error, NO_SPACE, SIZE_MAX, NULL);

    // null pointer
    error = buffer_constructor_const(NULL, 0, 0);
    buffer_test(buffer, error, NO_SELF, 0, NULL);
}

void constructor_array_test()
{
    // normal use
    u_char data_1[size_1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    error = buffer_constructor_array(&buffer, size_1, data_1);
    buffer_test(buffer, error, SUCCESS, size_1, data_1);

    // void buffer
    u_char data_2[size_2] = {};
    error = buffer_constructor_array(&buffer, size_2, data_2);
    buffer_test(buffer, error, SUCCESS, size_2, data_2);

    // to big buffer
    u_char data_3[1];
    error = buffer_constructor_array(&buffer, SIZE_MAX, data_3);
    buffer_test(buffer, error, NO_SPACE, SIZE_MAX, NULL);

    // null array
    error = buffer_constructor_array(&buffer, 10, NULL);
    buffer_test(buffer, error, ERROR_VALUE, 10, NULL);

    // null pointer
    error = buffer_constructor_array(NULL, 0, NULL);
    buffer_test(buffer, error, NO_SELF, 10, NULL);
}

void constructor_buffer_test()
{
    // normal use
    u_char data_1[] = {1, 2, 3};
    struct s_buffer buffer_1 = {3, data_1};
    error = buffer_constructor_buffer(&buffer, &buffer_1);
    buffer_test(buffer, error, SUCCESS, 3, data_1);

    // empty buffer
    u_char data_2[] = {};
    struct s_buffer buffer_2 = {0, data_2};
    error = buffer_constructor_buffer(&buffer, &buffer_2);
    buffer_test(buffer, error, SUCCESS, 0, data_2);

    // to big
    u_char data_3[] = {};
    struct s_buffer buffer_3 = {SIZE_MAX, data_3};
    error = buffer_constructor_buffer(&buffer, &buffer_3);
    buffer_test(buffer, error, NO_SPACE, SIZE_MAX, data_3);

    // null data
    struct s_buffer buffer_4 = {2, NULL};
    error = buffer_constructor_buffer(&buffer, &buffer_4);
    buffer_test(buffer, error, INTERNAL_ERROR, 2, NULL);

    // null buffer
    error = buffer_constructor_buffer(&buffer, NULL);
    buffer_test(buffer, error, ERROR_VALUE, 10, NULL);

    // null pointer
    error = buffer_constructor_buffer(NULL, &buffer_1);
    buffer_test(buffer, error, NO_SELF, 10, NULL);
}

void constructor_str_test()
{
    // normal use
    string_t str1 = "hello";
    error = buffer_constructor_str(&buffer, str1, false);
    buffer_test(buffer, error, SUCCESS, 5, (u_char *)str1);
    error = buffer_constructor_str(&buffer, str1, true);
    buffer_test(buffer, error, SUCCESS, 6, (u_char *)str1);

    // empty string
    string_t str2 = "";
    error = buffer_constructor_str(&buffer, str2, false);
    buffer_test(buffer, error, SUCCESS, 0, (u_char *)str2);
    error = buffer_constructor_str(&buffer, str2, true);
    buffer_test(buffer, error, SUCCESS, 1, (u_char *)str2);

    // null string
    error = buffer_constructor_str(&buffer, NULL, false);
    buffer_test(buffer, error, ERROR_VALUE, 1, (u_char *)str2);

    // no return pointer
    error = buffer_constructor_str(NULL, str2, false);
    buffer_test(buffer, error, NO_SELF, 0, NULL);

    // !TODO - add to big buffer
}

Test constructor_tests[] = {
    {"buffer_constructor_size", constructor_size_test},
    {"buffer_constructor_const", constructor_const_test},
    {"buffer_constructor_array", constructor_array_test},
    {"buffer_constructor_buffer", constructor_buffer_test},
    {"buffer_constructor_str", constructor_str_test}};

void test_constructor()
{
    run_tests(constructor_tests, sizeof(constructor_tests) / sizeof(Test), 2);
}
