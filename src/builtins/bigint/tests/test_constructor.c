#include "test_constructor.h"

Buffer buffer;
BigInt bigint;
int error;

void bigi_dest(BigInt bigint)
{
    if (bigint == NULL)
        return;

    buffer_destructor(&bigint->buffer);

    free(bigint);
}

void constructor_bigint_test(BigInt new_bigint, int new_error, int error, size_t exhibitor, bool sign, Buffer buffer)
{
    // check return error code
    assert_equal_ul("Error code", error, new_error);

    // if there normaly there is an error check if the pointer is null
    if (error != SUCCESS)
        assert_equal_p("Bigint pointer", NULL, new_bigint);
    // if it is propraly created check parameter
    else
    {
        // test the exhibitor and the sign
        assert_equal_ul("Exhibitor", exhibitor, new_bigint->exhibitor);
        assert_equal_b("Sign", sign, new_bigint->sign);

        // if a buffer is provided test it
        if (buffer != NULL)
            assert_equal_buffer("Buffer", buffer, new_bigint->buffer);
    }

    // destroy the bigint and the buffer
    bigi_dest(bigint);
    buffer_destructor(&buffer);
}

void constructor_null_test()
{
    // normal used
    if (buffer_constructor_const(&buffer, 0, 0) != SUCCESS)
        errx(2, "Error durring buffer creation");

    error = bigint_constructor_null(&bigint);
    constructor_bigint_test(bigint, error, SUCCESS, 0, POSITIVE, buffer);
    // !TODO - not enought memory error
}

void constructor_array_test()
{
    unsigned char array[] = {0x00, 0xf1, 0xff};
    // normal used
    if (buffer_constructor_array(&buffer, sizeof(array) / sizeof(unsigned char), array) != SUCCESS)
        errx(2, "Error durring buffer creation");
    error = bigint_constructor_null(&bigint);
    constructor_bigint_test(bigint, error, SUCCESS, 0, POSITIVE, buffer);
}

// private part of the tests
size_t _bigint_get_array_exhibitor(size_t size, unsigned char *array);

void get_array_exhibitor_test()
{
    unsigned char arr1[] = {0x00, 0x00};
    assert_equal_ul("empty array null size", 0, _bigint_get_array_exhibitor(0, arr1));
    assert_equal_ul("emty array size 1", 0, _bigint_get_array_exhibitor(1, arr1));
    assert_equal_ul("emty array size 2", 0, _bigint_get_array_exhibitor(2, arr1));

    unsigned char arr2[] = {0x01};
    assert_equal_ul("0x01 size 0", 0, _bigint_get_array_exhibitor(0, arr2));
    assert_equal_ul("0x01 size 1", 1, _bigint_get_array_exhibitor(1, arr2));

    unsigned char arr3[] = {0xf1};
    assert_equal_ul("0xf1 size 1", 8, _bigint_get_array_exhibitor(1, arr3));

    unsigned char arr4[] = {0x07, 0x00};
    assert_equal_ul("0xf100 size 1", 3, _bigint_get_array_exhibitor(1, arr4));
    assert_equal_ul("0xf100 size 2", 11, _bigint_get_array_exhibitor(2, arr4));
}

Test constructor_tests[] = {
    {"_bigint_get_array_exhibitor", get_array_exhibitor_test},
    {"bigint_constructor_null", constructor_null_test},
    {"bigint_constructor_array", constructor_array_test},
};

void test_constructor()
{
    run_tests(constructor_tests, sizeof(constructor_tests) / sizeof(Test), 2);
}
