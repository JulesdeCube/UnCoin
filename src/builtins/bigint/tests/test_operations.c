#include "test_operations.h"

BigInt bigint1;
BigInt bigint2;
bool res;
int error;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  PRIVATE                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int _bigint_comparison(BigInt bigint1, BigInt bigint2);
int _bigint_add(BigInt bigint1, BigInt bigint2, BigInt *result);
int _bigint_sub(BigInt bigint1, BigInt bigint2, BigInt *result);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   UTILS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void bigint_comparison_test(int res, int new_res, BigInt bigint1, BigInt bigint2)
{
    // check return result code
    assert_equal_i("Error code", res, new_res);

    bigint_destructor(&bigint1);
    bigint_destructor(&bigint2);

    putchar('|');
}

void bigint_add_private_test(int exprected_error, BigInt bigint1, BigInt bigint2, BigInt exprected_result)
{
    BigInt result;
    int error, res;

    char *b1, *b2, *title, *error_title;

    if (bigint1 != NULL)
    {
        error = bigint_to_string(bigint1, &b1, NULL);
        if (error != SUCCESS)
            errx(1, "can't create bigint1 : %i", error);
    }
    else
        b1 = "NULL";

    if (bigint2 != NULL)
    {

        error = bigint_to_string(bigint2, &b2, NULL);
        if (error != SUCCESS)
            errx(1, "can't create bigint2 : %i", error);
    }
    else
        b2 = "NULL";

    if (asprintf(&title, "%s + %s", b1, b2) == -1)
        errx(1, "cant create title test");

    if (asprintf(&error_title, "%s : wong error code", title) == -1)
        errx(1, "cant create error title test");

    res = _bigint_add(bigint1, bigint2, &result);
    assert_equal_i(error_title, exprected_error, res);

    if (res == SUCCESS)
        assert_equal_bigint(title, exprected_result, result);

    if (bigint1 != NULL)
        free(b1);
    if (bigint2 != NULL)
        free(b2);
    free(title);
    free(error_title);

    bigint_destructor(&bigint1);
    bigint_destructor(&bigint2);
    bigint_destructor(&result);
    bigint_destructor(&exprected_result);

    putchar('|');
}

void bigint_addition_test(int exprected_error, BigInt bigint1, BigInt bigint2, BigInt exprected_result)
{
    BigInt result;
    int error, res;

    char *b1, *b2, *title, *error_title;

    if (bigint1 != NULL)
    {
        error = bigint_to_string(bigint1, &b1, NULL);
        if (error != SUCCESS)
            errx(1, "can't create bigint1 : %i", error);
    }
    else
        b1 = "NULL";

    if (bigint2 != NULL)
    {

        error = bigint_to_string(bigint2, &b2, NULL);
        if (error != SUCCESS)
            errx(1, "can't create bigint2 : %i", error);
    }
    else
        b2 = "NULL";

    if (asprintf(&title, "%s + %s", b1, b2) == -1)
        errx(1, "cant create title test");

    if (asprintf(&error_title, "%s : wong error code", title) == -1)
        errx(1, "cant create error title test");

    res = bigint_addition(bigint1, bigint2, &result);
    assert_equal_i(error_title, exprected_error, res);

    if (res == SUCCESS)
        assert_equal_bigint(title, exprected_result, result);

    if (bigint1 != NULL)
        free(b1);
    if (bigint2 != NULL)
        free(b2);
    free(title);
    free(error_title);

    bigint_destructor(&bigint1);
    bigint_destructor(&bigint2);
    bigint_destructor(&result);
    bigint_destructor(&exprected_result);

    putchar('|');
}

void bigint_substract_private_test(int exprected_error, BigInt bigint1, BigInt bigint2, BigInt exprected_result)
{
    BigInt result;
    int error, res;

    char *b1, *b2, *title, *error_title;

    if (bigint1 != NULL)
    {
        error = bigint_to_string(bigint1, &b1, NULL);
        if (error != SUCCESS)
            errx(1, "can't create bigint1 : %i", error);
    }
    else
        b1 = "NULL";

    if (bigint2 != NULL)
    {

        error = bigint_to_string(bigint2, &b2, NULL);
        if (error != SUCCESS)
            errx(1, "can't create bigint2 : %i", error);
    }
    else
        b2 = "NULL";

    if (asprintf(&title, "%s - %s", b1, b2) == -1)
        errx(1, "cant create title test");

    if (asprintf(&error_title, "%s : wong error code", title) == -1)
        errx(1, "cant create error title test");

    res = _bigint_sub(bigint1, bigint2, &result);
    assert_equal_i(error_title, exprected_error, res);

    if (res == SUCCESS)
        assert_equal_bigint(title, exprected_result, result);

    if (bigint1 != NULL)
        free(b1);
    if (bigint2 != NULL)
        free(b2);
    free(title);
    free(error_title);

    bigint_destructor(&bigint1);
    bigint_destructor(&bigint2);
    bigint_destructor(&result);
    bigint_destructor(&exprected_result);

    putchar('|');
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   PUBLIC                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void bigint_comparison_tests()
{
    int result;

    //test with same sign same exhibitor
    //positive
    bigint_constructor_from_int(&bigint1, 2);
    bigint_constructor_from_int(&bigint2, 3);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(-1, result, bigint1, bigint2);

    bigint_constructor_from_int(&bigint1, 3);
    bigint_constructor_from_int(&bigint2, 2);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(1, result, bigint1, bigint2);

    bigint_constructor_from_int(&bigint1, 3);
    bigint_constructor_from_int(&bigint2, 3);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(0, result, bigint1, bigint2);

    //negative
    bigint_constructor_from_int(&bigint1, -3);
    bigint_constructor_from_int(&bigint2, -2);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(-1, result, bigint1, bigint2);

    bigint_constructor_from_int(&bigint1, -2);
    bigint_constructor_from_int(&bigint2, -3);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(1, result, bigint1, bigint2);

    bigint_constructor_from_int(&bigint1, -2);
    bigint_constructor_from_int(&bigint2, -2);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(0, result, bigint1, bigint2);

    //test with different sign same exh
    bigint_constructor_from_int(&bigint1, -2);
    bigint_constructor_from_int(&bigint2, 3);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(-1, result, bigint1, bigint2);

    bigint_constructor_from_int(&bigint1, 2);
    bigint_constructor_from_int(&bigint2, -3);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(1, result, bigint1, bigint2);

    bigint_constructor_from_int(&bigint1, -2);
    bigint_constructor_from_int(&bigint2, 2);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(-1, result, bigint1, bigint2);

    bigint_constructor_from_int(&bigint1, 2);
    bigint_constructor_from_int(&bigint2, -2);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(-1, result, bigint1, bigint2);

    //different size and exhib
    bigint_constructor_from_int(&bigint1, -2);
    bigint_constructor_from_int(&bigint2, 10);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(-1, result, bigint1, bigint2);

    bigint_constructor_from_int(&bigint1, 2);
    bigint_constructor_from_int(&bigint2, -10);
    result = _bigint_comparison(bigint1, bigint2);
    bigint_comparison_test(1, result, bigint1, bigint2);
}

void bigint_add_private_tests()
{
    BigInt result;

    bigint_constructor_from_int(&bigint1, 2);
    bigint_constructor_from_int(&bigint2, 4);
    bigint_constructor_from_int(&result, 6);
    bigint_add_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0);
    bigint_constructor_from_int(&bigint2, 0);
    bigint_constructor_from_int(&result, 0);
    bigint_add_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xff);
    bigint_constructor_from_int(&bigint2, 0x1);
    bigint_constructor_from_int(&result, 0x100);
    bigint_add_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xffff);
    bigint_constructor_from_int(&bigint2, 0x1);
    bigint_constructor_from_int(&result, 0x10000);
    bigint_add_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xff02);
    bigint_constructor_from_int(&bigint2, 0xff);
    bigint_constructor_from_int(&result, 0x10001);
    bigint_add_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x4386788);
    bigint_constructor_from_int(&bigint2, 0x6a011004);
    bigint_constructor_from_int(&result, 0x4386788 + 0x6a011004);
    bigint_add_private_test(SUCCESS, bigint1, bigint2, result);


}

void bigint_substract_private_tests()
{
    BigInt result;
    bigint_constructor_from_int(&bigint1, 0);
    bigint_constructor_from_int(&bigint2, 0);
    bigint_constructor_from_int(&result, 0);
    bigint_substract_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 6);
    bigint_constructor_from_int(&bigint2, 4);
    bigint_constructor_from_int(&result, 2);
    bigint_substract_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 6);
    bigint_constructor_from_int(&bigint2, 4);
    bigint_constructor_from_int(&result, 2);
    bigint_substract_private_test(SUCCESS, bigint1, bigint2, result);
}

void bigint_addition_tests()
{

    BigInt result;

    for (int i = -3; i <= 3; i++)
    {
        for (int j = -3; j <= 3; j++)
        {
            bigint_constructor_from_int(&bigint1, i);
            bigint_constructor_from_int(&bigint2, j);
            bigint_constructor_from_int(&result, i + j);
            bigint_addition_test(SUCCESS, bigint1, bigint2, result);
        }
    }
    bigint_constructor_from_int(&bigint1, 2);
    bigint_constructor_from_int(&bigint2, 4);
    bigint_constructor_from_int(&result, 6);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0);
    bigint_constructor_from_int(&bigint2, 0);
    bigint_constructor_from_int(&result, 0);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xff);
    bigint_constructor_from_int(&bigint2, 0x1);
    bigint_constructor_from_int(&result, 0x100);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xffff);
    bigint_constructor_from_int(&bigint2, 0x1);
    bigint_constructor_from_int(&result, 0x10000);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xff02);
    bigint_constructor_from_int(&bigint2, 0xff);
    bigint_constructor_from_int(&result, 0x10001);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x4386788);
    bigint_constructor_from_int(&bigint2, 0x6a011004);
    bigint_constructor_from_int(&result, 0x4386788 + 0x6a011004);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 1);
    bigint_constructor_from_int(&bigint2, -1);
    bigint_constructor_from_int(&result, 0);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, -1);
    bigint_constructor_from_int(&bigint2, -1);
    bigint_constructor_from_int(&result, -2);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    /*
    bigint_constructor_from_int(&bigint1, 1);
    bigint_constructor_from_int(&bigint2, -20);
    bigint_constructor_from_int(&result, -19);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, -20);
    bigint_constructor_from_int(&bigint2, 1);
    bigint_constructor_from_int(&result, -19);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);*/

    bigint_constructor_from_int(&bigint1, -10);
    bigint_constructor_from_int(&bigint2, -1);
    bigint_constructor_from_int(&result, -11);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x4386788);
    bigint_addition_test(NO_SELF, bigint1, NULL, NULL);

    bigint_constructor_from_int(&bigint2, 0x4386788);
    bigint_addition_test(NO_SELF, NULL, bigint2, NULL);

    error = bigint_addition(NULL, NULL, NULL);
    assert_equal_i("no return pointer", ERROR_VALUE, error);
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   TESTS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Test operations_tests[] = {
    //{"_bigint_comparison", bigint_comparison_tests},
    {"_bigint_subtract", bigint_substract_private_tests},
    {"_bigint_add", bigint_add_private_tests},
    {"bigint_addition", bigint_addition_tests}};

void test_operations()
{
    run_tests(operations_tests, sizeof(operations_tests) / sizeof(Test), 2);
}
