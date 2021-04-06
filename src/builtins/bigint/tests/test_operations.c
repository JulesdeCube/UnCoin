#include "test_operations.h"

BigInt bigint1;
BigInt bigint2;
bool res;
int result;
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   UTILS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void bigi_destr(BigInt bigint)
{
    if (bigint == NULL)
        return;

    buffer_destructor(&bigint->buffer);

    free(bigint);
}

void bigint_comparison_test(int res, int new_res,BigInt bigint1, BigInt bigint2)
{
    // check return error code
    assert_equal_i("Error code", res, new_res);

    bigi_destr(bigint1);
    bigi_destr(bigint2);

    putchar('|');
}
void bigint_compare_test(bool res, bool new_res,BigInt bigint1, BigInt bigint2)
{
    // check return error code
    assert_equal_i("Error code", res, new_res);

    bigi_destr(bigint1);
    bigi_destr(bigint2);

    putchar('|');
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
void bigint_comparison_tests()
{
    //test with same sign same exhibitor
    //positive
    bigint_constructor_from_int(&bigint1,2);
    bigint_constructor_from_int(&bigint2,3);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(-1,result,bigint1,bigint2);

    bigint_constructor_from_int(&bigint1,3);
    bigint_constructor_from_int(&bigint2,2);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(1,result,bigint1,bigint2);

    bigint_constructor_from_int(&bigint1,3);
    bigint_constructor_from_int(&bigint2,3);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(0,result,bigint1,bigint2);

    //negative
    bigint_constructor_from_int(&bigint1,-3);
    bigint_constructor_from_int(&bigint2,-2);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(-1,result,bigint1,bigint2);

    bigint_constructor_from_int(&bigint1,-2);
    bigint_constructor_from_int(&bigint2,-3);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(1,result,bigint1,bigint2);

    bigint_constructor_from_int(&bigint1,-2);
    bigint_constructor_from_int(&bigint2,-2);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(0,result,bigint1,bigint2);

    //test with different sign same exh
    bigint_constructor_from_int(&bigint1,-2);
    bigint_constructor_from_int(&bigint2,3);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(-1,result,bigint1,bigint2);

    bigint_constructor_from_int(&bigint1,2);
    bigint_constructor_from_int(&bigint2,-3);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(1,result,bigint1,bigint2);

    bigint_constructor_from_int(&bigint1,-2);
    bigint_constructor_from_int(&bigint2,2);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(-1,result,bigint1,bigint2);

    bigint_constructor_from_int(&bigint1,2);
    bigint_constructor_from_int(&bigint2,-2);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(-1,result,bigint1,bigint2);

    //different size and exhib
    bigint_constructor_from_int(&bigint1,-2);
    bigint_constructor_from_int(&bigint2,10);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(-1,result,bigint1,bigint2);

    bigint_constructor_from_int(&bigint1,2);
    bigint_constructor_from_int(&bigint2,-10);
    result = _bigint_comparison(bigint1,bigint2);
    bigint_comparison_test(1,result,bigint1,bigint2);

}

void bigint_compare_tests()
{

}


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   TESTS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Test operations_tests[] =
{
    {"_bigint_comparison", bigint_comparison_tests}
};

void test_operations()
{
    run_tests(operations_tests, sizeof(operations_tests) / sizeof(Test), 2);
}
