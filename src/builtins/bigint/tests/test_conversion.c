#include "test_constructor.h"

BigInt bigint;
int error;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   UTILS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void test_to_bool(char *name, BigInt bigint, int expected_error, bool exepted_value)
{
    bool result;

    int return_code = bigint_to_bool(bigint, &result);
    char *str;

    asprintf(&str, "%s: error code", name);
    assert_equal_i(str, expected_error, return_code);
    free(str);
    if (return_code == SUCCESS)
    {
        asprintf(&str, "%s: value", name);
        assert_equal_b(str, exepted_value, result);
        free(str);
    }

    bigint_destructor(&bigint);
    putchar('|');
}

void test_to_int(char *name, BigInt bigint, int expected_error, int exepted_value)
{
    int result;

    int return_code = bigint_to_int(bigint, &result);
    char *str;

    asprintf(&str, "%s: error code", name);
    assert_equal_i(str, expected_error, return_code);
    free(str);
    if (return_code == SUCCESS || return_code == NO_SPACE)
    {
        asprintf(&str, "%s: value", name);
        assert_equal_i(str, exepted_value, result);
        free(str);
    }

    bigint_destructor(&bigint);
    putchar('|');
}

BigInt create_safe_bigint(bool sign, size_t size, u_char *array)
{
    BigInt new_bigint;
    int error = bigint_constructor_array(&new_bigint, sign, size, array);
    if (error != SUCCESS)
        errx(2, "Error durring buffer creation : %i", error);
    return new_bigint;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  PRIVATE                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// private part of the tests

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   PUBLIC                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void to_bool_test()
{
    bigint = create_safe_bigint(POSITIVE, 0, NULL + 1);
    test_to_bool("null positive", bigint, SUCCESS, false);

    bigint = create_safe_bigint(NEGATIVE, 0, NULL + 1);
    test_to_bool("null negavie", bigint, SUCCESS, false);

    u_char arr1[] = {0x00};
    bigint = create_safe_bigint(NEGATIVE, 1, arr1);
    test_to_bool("null with size", bigint, SUCCESS, false);

    u_char arr2[] = {0x01};
    bigint = create_safe_bigint(NEGATIVE, 1, arr2);
    test_to_bool("0x01", bigint, SUCCESS, true);

    u_char arr3[] = {0xf0, 0x12, 0x01};
    bigint = create_safe_bigint(NEGATIVE, 3, arr3);
    test_to_bool("0xf01201", bigint, SUCCESS, true);

    u_char arr4[] = {0xf0, 0x12, 0x00};
    bigint = create_safe_bigint(NEGATIVE, 3, arr4);
    test_to_bool("0xf01200", bigint, SUCCESS, true);

    // error
    test_to_bool("null positive", NULL, NO_SELF, true);

    assert_equal_i("", ERROR_VALUE, bigint_to_bool(NULL + 1, NULL));
    putchar('|');
}

void to_int_test()
{
    bigint = create_safe_bigint(POSITIVE, 0, NULL + 1);
    test_to_int("null positive", bigint, SUCCESS, 0);

    bigint = create_safe_bigint(NEGATIVE, 0, NULL + 1);
    test_to_int("null negavie", bigint, SUCCESS, 0);

    u_char arr1[] = {0x00};
    bigint = create_safe_bigint(NEGATIVE, 1, arr1);
    test_to_int("null with small size", bigint, SUCCESS, 0);

    u_char arr2[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bigint = create_safe_bigint(NEGATIVE, 1, arr2);
    test_to_int("null with big size", bigint, SUCCESS, 0);

    u_char arr3[] = {0x01};
    bigint = create_safe_bigint(POSITIVE, 1, arr3);
    test_to_int("0x01", bigint, SUCCESS, 1);

    bigint = create_safe_bigint(NEGATIVE, 1, arr3);
    test_to_int("-0x01", bigint, SUCCESS, -1);

    u_char arr4[] = {0xaf};
    bigint = create_safe_bigint(NEGATIVE, 1, arr4);
    test_to_int("-0xaf", bigint, SUCCESS, -0xaf);

    u_char arr5[] = {0xfa, 0x12};
    bigint = create_safe_bigint(POSITIVE, 2, arr5);
    test_to_int("0x12fa", bigint, SUCCESS, 0x12fa);

    u_char arr6[] = {0xfa, 0x00, 0xda, 0x35, 0x04, 0x8b};
    bigint = create_safe_bigint(POSITIVE, 4, arr6);
    test_to_int("0x35da00fa", bigint, SUCCESS, 0x35da00fa);

    bigint = create_safe_bigint(POSITIVE, 6, arr6);
    test_to_int("0x8b0435da00fa", bigint, NO_SPACE, 0x35da00fa);

    test_to_int("null bigint", NULL, NO_SELF, 0);

    assert_equal_i("null pointer: return code",
                   ERROR_VALUE,
                   bigint_to_int(NULL + 1, NULL));
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   TESTS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Test conversion_tests[] = {
    {"bigint_to_bool", to_bool_test},
    {"bigint_to_int", to_int_test}};

void test_conversion()
{
    run_tests(conversion_tests, sizeof(conversion_tests) / sizeof(Test), 2);
}
