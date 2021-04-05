#include "test_operations.h"

Buffer buffer;
BigInt bigint1;
BigInt bigint2;
Buffer buffer_res;
int error;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   UTILS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



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
void bigint_comparison_test()
{
    //test with same sign same exhibitor

    bigint_constructor_from_int(&bigint1,1);
    bigint_constructor_from_int(&bigint1,2);
    assert_equal_i("comparion between 1 2",-1,_bigint_comparison(bigint1,bigint2));

    bigint_destructor(&bigint1);
    bigint_destructor(&bigint2);
    bigint_constructor_from_int(&bigint1,1);
    bigint_constructor_from_int(&bigint1,2);
    assert_equal_i("comparion between 2 1",-1,_bigint_comparison(bigint1,bigint2));

}


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   TESTS                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Test destructor_tests[] =
{

};

void test_operations()
{
    run_tests(destructor_tests, sizeof(destructor_tests) / sizeof(Test), 2);
}
