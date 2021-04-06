#include "test_constructor.h"

Buffer buffer;
BigInt bigint;
Buffer buffer_res;
int error;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   UTILS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void bigi_dest(BigInt bigint)
{
    if (bigint == NULL)
        return;

    buffer_destructor(&bigint->buffer);

    free(bigint);
}

void bigint_constructor_test(BigInt new_bigint, int new_error, int error,
                             size_t exhibitor, bool sign, Buffer buffer)
{
    // check return error code
    assert_equal_i("Error code", error, new_error);

    // if there normaly there is an error check if the pointer is null
    if (error != SUCCESS)
    {
        if (error != NO_SELF)
            assert_equal_p("Bigint pointer", NULL, new_bigint);
    }
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
    if (error != NO_SELF)
        bigi_dest(bigint);
    buffer_destructor(&buffer);
    putchar('|');
}

Buffer create_safe_buffer(size_t size, u_char *array)
{
    Buffer new_buffer;
    int error = buffer_constructor_array(&new_buffer, size, array);
    if (error != SUCCESS)
        errx(2, "Error durring buffer creation : %i", error);
    return new_buffer;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  PRIVATE                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// private part of the tests
size_t _bigint_get_array_exhibitor(size_t size, unsigned char *array);

void get_array_exhibitor_test()
{
    unsigned char arr1[] = {0x00, 0x00};
    assert_equal_ul("empty size 0", 0, _bigint_get_array_exhibitor(0, arr1));
    assert_equal_ul("emtpy size 1", 0, _bigint_get_array_exhibitor(1, arr1));
    assert_equal_ul("emtpy size 2", 0, _bigint_get_array_exhibitor(2, arr1));
    putchar('|');

    unsigned char arr2[] = {0x01};
    assert_equal_ul("0x01 size 0", 0, _bigint_get_array_exhibitor(0, arr2));
    assert_equal_ul("0x01 size 1", 1, _bigint_get_array_exhibitor(1, arr2));
    putchar('|');

    unsigned char arr3[] = {0xf1};
    assert_equal_ul("0xf1 size 1", 8, _bigint_get_array_exhibitor(1, arr3));
    putchar('|');

    unsigned char arr4[] = {0x00, 0x07};
    assert_equal_ul("0x0700 size 1", 0, _bigint_get_array_exhibitor(1, arr4));
    assert_equal_ul("0x0700 size 2", 11, _bigint_get_array_exhibitor(2, arr4));
    putchar('|');

    u_char arr5[] = {0xff, 0x01};
    assert_equal_ul("0x01ff size 1", 8, _bigint_get_array_exhibitor(1, arr5));
    assert_equal_ul("0x01ff size 2", 9, _bigint_get_array_exhibitor(2, arr5));
    putchar('|');

    u_char arr6[] = {0xfa, 0x1f, 0x00};
    assert_equal_ul("0x001f|fa size 1", 8, _bigint_get_array_exhibitor(1, arr6));
    assert_equal_ul("0x00|1ffa size 2", 13, _bigint_get_array_exhibitor(2, arr6));
    assert_equal_ul("0x|001ffa size 3", 13, _bigint_get_array_exhibitor(3, arr6));
    putchar('|');

    u_char arr7[] = {0xfa, 0x12};
    assert_equal_ul("0x12fa", 13, _bigint_get_array_exhibitor(2, arr7));
    putchar('|');
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   PUBLIC                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void constructor_null_test()
{
    // normal used
    if (buffer_constructor_const(&buffer, 0, 0) != SUCCESS)
        errx(2, "Error durring buffer creation");

    error = bigint_constructor_null(&bigint);
    bigint_constructor_test(bigint, error, SUCCESS, 0, POSITIVE, buffer);

    // error no return buffer
    error = bigint_constructor_null(NULL);
    bigint_constructor_test(bigint, error, NO_SELF, 0, POSITIVE, NULL);
    // !TODO - not enought memory error
}

void constructor_array_test()
{
    u_char array[] = {0xff, 0xf1, 0x00};
    // normal used positive with even exhibitor
    buffer = create_safe_buffer(2, array);
    error = bigint_constructor_array(&bigint, POSITIVE, 3, array);
    bigint_constructor_test(bigint, error, SUCCESS, 16, POSITIVE, buffer);

    // normal used negative
    buffer = create_safe_buffer(2, array);
    error = bigint_constructor_array(&bigint, NEGATIVE, 3, array);
    bigint_constructor_test(bigint, error, SUCCESS, 16, NEGATIVE, buffer);

    u_char array2[] = {0xff, 0x01};
    // normal used positive not even exhibitor
    buffer = create_safe_buffer(2, array2);
    error = bigint_constructor_array(&bigint, POSITIVE, 2, array2);
    bigint_constructor_test(bigint, error, SUCCESS, 9, POSITIVE, buffer);

    // normale use 2
    u_char array3[] = {0xfa, 0x12};
    buffer = create_safe_buffer(2, array3);
    error = bigint_constructor_array(&bigint, POSITIVE, 2, array3);
    bigint_constructor_test(bigint, error, SUCCESS, 13, POSITIVE, buffer);

    // no buffer return
    error = bigint_constructor_array(NULL, POSITIVE, 0, NULL);
    bigint_constructor_test(bigint, error, NO_SELF, 0, POSITIVE, NULL);
    // wong sign
    error = bigint_constructor_array(&bigint, 2, 0, NULL);
    bigint_constructor_test(bigint, error, ERROR_VALUE, 0, POSITIVE, NULL);
    // no array
    error = bigint_constructor_array(&bigint, POSITIVE, 0, NULL);
    bigint_constructor_test(bigint, error, ERROR_VALUE, 0, POSITIVE, NULL);

    // !TODO - check for no space error
}

void constructor_buffer_test()
{
    u_char arr1[] = {0x11, 0x10, 0x00};
    // normalize size
    buffer = create_safe_buffer(2, arr1);
    error = bigint_constructor_buffer(&bigint, POSITIVE, buffer);
    bigint_constructor_test(bigint, error, SUCCESS, 13, POSITIVE, buffer);

    // not normaize size
    buffer = create_safe_buffer(3, arr1);
    buffer_res = create_safe_buffer(2, arr1);
    error = bigint_constructor_buffer(&bigint, POSITIVE, buffer);
    bigint_constructor_test(bigint, error, SUCCESS, 13, POSITIVE, buffer_res);
    buffer_destructor(&buffer);

    // null buffer size
    u_char arr2[] = {0x00, 0x00, 0x00};
    buffer = create_safe_buffer(3, arr2);
    buffer_res = create_safe_buffer(0, arr2);
    error = bigint_constructor_buffer(&bigint, POSITIVE, buffer);
    bigint_constructor_test(bigint, error, SUCCESS, 0, POSITIVE, buffer_res);
    buffer_destructor(&buffer);

    // null buffer size
    buffer = create_safe_buffer(3, arr2);
    buffer_res = create_safe_buffer(0, arr2);
    error = bigint_constructor_buffer(&bigint, POSITIVE, buffer);
    bigint_constructor_test(bigint, error, SUCCESS, 0, POSITIVE, buffer_res);
    buffer_destructor(&buffer);

    // no return pointer
    error = bigint_constructor_buffer(NULL, POSITIVE, buffer);
    bigint_constructor_test(bigint, error, NO_SELF, 0, POSITIVE, NULL);

    // wrong sign
    error = bigint_constructor_buffer(&bigint, 2, buffer);
    bigint_constructor_test(bigint, error, ERROR_VALUE, 0, POSITIVE, NULL);

    // no buffer
    error = bigint_constructor_buffer(&bigint, POSITIVE, NULL);
    bigint_constructor_test(bigint, error, ERROR_VALUE, 0, POSITIVE, NULL);
}

void constructor_buffer_signed_test()
{
    printf("TODO\n");
}

void constructor_bigint_test()
{
    printf("TODO\n");
}

void constructor_bigint_from_int()
{
    //positive sign
    error = bigint_constructor_from_int(&bigint, 0);
    bigint_constructor_test(bigint, error, SUCCESS, 0, POSITIVE, NULL);

    error = bigint_constructor_from_int(&bigint, 1);
    bigint_constructor_test(bigint, error, SUCCESS, 1, POSITIVE, NULL);

    error = bigint_constructor_from_int(&bigint, 10);
    bigint_constructor_test(bigint, error, SUCCESS, 4, POSITIVE, NULL);

    error = bigint_constructor_from_int(&bigint, 1000);
    bigint_constructor_test(bigint, error, SUCCESS, 10, POSITIVE, NULL);

    //negative sign
    error = bigint_constructor_from_int(&bigint, -1);
    bigint_constructor_test(bigint, error, SUCCESS, 1, NEGATIVE, NULL);

    error = bigint_constructor_from_int(&bigint, -10);
    bigint_constructor_test(bigint, error, SUCCESS, 4, NEGATIVE, NULL);

    error = bigint_constructor_from_int(&bigint, -1000);
    bigint_constructor_test(bigint, error, SUCCESS, 10, NEGATIVE, NULL);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   TESTS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Test constructor_tests[] = {
    {"_bigint_get_array_exhibitor", get_array_exhibitor_test},
    {"bigint_constructor_array", constructor_array_test},
    {"bigint_constructor_null", constructor_null_test},
    {"bigint_constructor_buffer", constructor_buffer_test},
    {"bigint_constructor_buffer_signed", constructor_buffer_signed_test},
    {"bigint_constructor_bigint", constructor_bigint_test},
    {"constructor_bigint_from_int", constructor_bigint_from_int}};

void test_constructor()
{
    run_tests(constructor_tests, sizeof(constructor_tests) / sizeof(Test), 2);
}
