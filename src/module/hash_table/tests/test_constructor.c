#include "test_constructor.h"

int error;

void htab_test(Htab new_htab, int new_code, int error, size_t size)
{
    // check the error code
    assert_equal_ul("Error code", error, new_code);
    // if it's not a success check if the htab is correcly set
    if (error != SUCCESS)
        assert_equal_p("Htab pointer", NULL, new_htab);
    // if all wen't good do the htab check
    else
    {
        // test the size
        assert_equal_ul("Size", size, new_htab->size);
        // free the htab
        htab_destructor(new_htab);
    }
    // add a htab test separator
    putchar('|');
}

void constructor_test()
{
    Htab htab;
    error = htab_constructor(&htab);
    htab_test(htab, error, SUCCESS, 0);

}

Test hash_tests[] = {
    {"constructor", constructor_test}
    };

void test_constructor()
{
    run_tests(hash_tests, sizeof(hash_tests) / sizeof(Test), 2);
}
