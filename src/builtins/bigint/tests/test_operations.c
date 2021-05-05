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

void bigint_substract_test(int exprected_error, BigInt bigint1, BigInt bigint2, BigInt exprected_result)
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

    res = bigint_substraction(bigint1, bigint2, &result);
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

void bigint_mul_private_test(int expected_error, BigInt bigint1, BigInt bigint2, BigInt expected_result)
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

    if (asprintf(&title, "%s * %s", b1, b2) == -1)
        errx(1, "cant create title test");

    if (asprintf(&error_title, "%s : wong error code", title) == -1)
        errx(1, "cant create error title test");

    res = bigint_multiplication(bigint1, bigint2, &result);
    assert_equal_i(error_title, expected_error, res);

    if (res == SUCCESS)
        assert_equal_bigint(title, expected_result, result);

    if (bigint1 != NULL)
        free(b1);
    if (bigint2 != NULL)
        free(b2);
    free(title);
    free(error_title);

    bigint_destructor(&bigint1);
    bigint_destructor(&bigint2);
    bigint_destructor(&result);
    bigint_destructor(&expected_result);

    putchar('|');
}

void bigint_left_shift_test(int exprected_error, BigInt bigint, size_t shift, BigInt exprected_result)
{
    BigInt result;
    int error, res;

    char *str, *title, *error_title;

    if (bigint != NULL)
    {
        error = bigint_to_string(bigint, &str, NULL);
        if (error != SUCCESS)
            errx(1, "can't create bigint : %i", error);
    }
    else
        str = "NULL";

    if (asprintf(&title, "%s << %lu", str, shift) == -1)
        errx(1, "cant create title test");

    if (asprintf(&error_title, "%s : wong error code", title) == -1)
        errx(1, "cant create error title test");

    res = bigint_left_shift(bigint, shift, &result);
    assert_equal_i(error_title, exprected_error, res);

    if (res == SUCCESS)
        assert_equal_bigint(title, exprected_result, result);

    if (bigint != NULL)
        free(str);
    free(title);
    free(error_title);

    bigint_destructor(&bigint);
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
    bigint_comparison_test(1, result, bigint1, bigint2);

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

    bigint_constructor_from_int(&bigint1, 1);
    bigint_constructor_from_int(&bigint2, -2);
    bigint_constructor_from_int(&result, -1);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);
    /*
    bigint_constructor_from_int(&bigint1, -20);
    bigint_constructor_from_int(&bigint2, 1);
    bigint_constructor_from_int(&result, -19);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);*/

    bigint_constructor_from_int(&bigint1, -10);
    bigint_constructor_from_int(&bigint2, -1);
    bigint_constructor_from_int(&result, -11);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x0);
    bigint_constructor_from_int(&bigint2, 0xffff);
    bigint_constructor_from_int(&result, 0xffff);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xffff);
    bigint_constructor_from_int(&bigint2, 0x1fffe);
    bigint_constructor_from_int(&result, 0x2fffd);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x2fffd);
    bigint_constructor_from_int(&bigint2, 0x3fffc);
    bigint_constructor_from_int(&result, 0x6fff9);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x6fff9);
    bigint_constructor_from_int(&bigint2, 0x7fff8);
    bigint_constructor_from_int(&result, 0xefff1);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xefff1);
    bigint_constructor_from_int(&bigint2, 0xffff0);
    bigint_constructor_from_int(&result, 0x1effe1);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x1effe1);
    bigint_constructor_from_int(&bigint2, 0x1fffe0);
    bigint_constructor_from_int(&result, 0x3effc1);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x3effc1);
    bigint_constructor_from_int(&bigint2, 0x3fffc0);
    bigint_constructor_from_int(&result, 0x7eff81);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x7eff81);
    bigint_constructor_from_int(&bigint2, 0x7fff80);
    bigint_constructor_from_int(&result, 0xfeff01);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xfeff01);
    bigint_constructor_from_int(&bigint2, 0xffff00);
    bigint_constructor_from_int(&result, 0x1fefe01);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x1fefe01);
    bigint_constructor_from_int(&bigint2, 0x1fffe00);
    bigint_constructor_from_int(&result, 0x3fefc01);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x3fefc01);
    bigint_constructor_from_int(&bigint2, 0x3fffc00);
    bigint_constructor_from_int(&result, 0x7fef801);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x7fef801);
    bigint_constructor_from_int(&bigint2, 0x7fff800);
    bigint_constructor_from_int(&result, 0xffef001);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xffef001);
    bigint_constructor_from_int(&bigint2, 0xffff000);
    bigint_constructor_from_int(&result, 0x1ffee001);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x1ffee001);
    bigint_constructor_from_int(&bigint2, 0x1fffe000);
    bigint_constructor_from_int(&result, 0x3ffec001);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x3ffec001);
    bigint_constructor_from_int(&bigint2, 0x3fffc000);
    bigint_constructor_from_int(&result, 0x7ffe8001);
    bigint_addition_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x4386788);
    bigint_addition_test(NO_SELF, bigint1, NULL, NULL);

    bigint_constructor_from_int(&bigint2, 0x4386788);
    bigint_addition_test(NO_SELF, NULL, bigint2, NULL);

    error = bigint_addition(NULL, NULL, NULL);
    assert_equal_i("no return pointer", ERROR_VALUE, error);
}

void bigint_substract_tests()
{

    BigInt result;

    for (int i = -3; i <= 3; i++)
    {
        for (int j = -3; j <= 3; j++)
        {
            bigint_constructor_from_int(&bigint1, i);
            bigint_constructor_from_int(&bigint2, j);
            bigint_constructor_from_int(&result, i - j);
            bigint_substract_test(SUCCESS, bigint1, bigint2, result);
        }
    }

    bigint_constructor_from_int(&bigint1, 2);
    bigint_constructor_from_int(&bigint2, 4);
    bigint_constructor_from_int(&result, -2);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0);
    bigint_constructor_from_int(&bigint2, 0);
    bigint_constructor_from_int(&result, 0);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x100);
    bigint_constructor_from_int(&bigint2, 0xff);
    bigint_constructor_from_int(&result, 0x01);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, -0xffff);
    bigint_constructor_from_int(&bigint2, 0x1);
    bigint_constructor_from_int(&result, -0x10000);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0xff02);
    bigint_constructor_from_int(&bigint2, 0xff);
    bigint_constructor_from_int(&result, 0xfe03);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x4386788);
    bigint_constructor_from_int(&bigint2, 0x6a011004);
    bigint_constructor_from_int(&result, 0x4386788 - 0x6a011004);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 1);
    bigint_constructor_from_int(&bigint2, -1);
    bigint_constructor_from_int(&result, 2);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, -1);
    bigint_constructor_from_int(&bigint2, -1);
    bigint_constructor_from_int(&result, 0);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    /*
    bigint_constructor_from_int(&bigint1, 1);
    bigint_constructor_from_int(&bigint2, -20);
    bigint_constructor_from_int(&result, -19);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, -20);
    bigint_constructor_from_int(&bigint2, 1);
    bigint_constructor_from_int(&result, -19);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);*/

    bigint_constructor_from_int(&bigint1, -10);
    bigint_constructor_from_int(&bigint2, -1);
    bigint_constructor_from_int(&result, -9);
    bigint_substract_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 0x4386788);
    bigint_substract_test(NO_SELF, bigint1, NULL, NULL);

    bigint_constructor_from_int(&bigint2, 0x4386788);
    bigint_substract_test(NO_SELF, NULL, bigint2, NULL);

    error = bigint_substraction(NULL, NULL, NULL);
    assert_equal_i("no return pointer", ERROR_VALUE, error);
}

void bigint_left_shift_tests()
{
    BigInt result;

    for (size_t i = 1; i < 32; i *= 3)
        for (size_t j = 0; j < 27; j++)
        {
            bigint_constructor_from_int(&bigint1, i);
            bigint_constructor_from_int(&result, i << j);
            bigint_left_shift_test(SUCCESS, bigint1, j, result);
        }

    bigint_constructor_from_int(&bigint1, 0);
    bigint_constructor_from_int(&result, 0);
    bigint_left_shift_test(SUCCESS, bigint1, 100, result);

    bigint_constructor_from_int(&bigint1, 0xff);
    bigint_constructor_from_int(&result, 0xff00);
    bigint_left_shift_test(SUCCESS, bigint1, 8, result);

    bigint_constructor_from_int(&bigint1, 0x1324);
    bigint_constructor_from_int(&result, 0x1324 << 16);
    bigint_left_shift_test(SUCCESS, bigint1, 16, result);

    bigint_constructor_from_int(&bigint1, 30);
    bigint_constructor_from_int(&result, 30 << 5);
    bigint_left_shift_test(SUCCESS, bigint1, 5, result);
}

void bigint_mul_private_tests()
{
    BigInt result;
    bigint_constructor_from_int(&bigint1, 1);
    bigint_constructor_from_int(&bigint2, 2);
    bigint_constructor_from_int(&result, 2);
    bigint_mul_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 1);
    bigint_constructor_from_int(&bigint2, 1000);
    bigint_constructor_from_int(&result, 1000);
    bigint_mul_private_test(SUCCESS, bigint1, bigint2, result);

    /*
    bigint_constructor_from_int(&bigint1, 100);
    bigint_constructor_from_int(&bigint2, 4);
    bigint_constructor_from_int(&result, 400);
    bigint_mul_private_test(SUCCESS, bigint1, bigint2, result);

    bigint_constructor_from_int(&bigint1, 2);
    bigint_constructor_from_int(&bigint2, 1);
    bigint_constructor_from_int(&result, 2);
    bigint_mul_private_test(SUCCESS, bigint1, bigint2, result);*/
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   TESTS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Test operations_tests[] = {
    {"_bigint_comparison", bigint_comparison_tests},
    {"_bigint_subtract", bigint_substract_private_tests},
    {"_bigint_add", bigint_add_private_tests},
    {"bigint_addition", bigint_addition_tests},
    {"bigint_subtract", bigint_substract_tests},
    {"bigint_left_shift", bigint_left_shift_tests},
    {"bigint_multiplication", bigint_mul_private_tests}};

void test_operations()
{
    run_tests(operations_tests, sizeof(operations_tests) / sizeof(Test), 2);
}
