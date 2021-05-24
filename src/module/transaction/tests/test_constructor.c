#include "test_constructor.h"

int error, r_error;

void trans_test(Transaction t, Buffer from, Buffer to, Buffer privkey, Buffer date, long amount, int error, int new_code)
{
    // check the error code
    assert_equal_ul("Error code", error, new_code);

    assert_equal_buffer("From", from, t->from);
    assert_equal_buffer("To", to, t->to);
    assert_equal_buffer("Date", date, t->date);
    //assert_equal_buffer("Signature", , t->signature);
    buffer_destructor_safe(&privkey);
    buffer_destructor_safe(&date);

    assert_equal_ul("Amount", (size_t) amount, (size_t) t->amount);

    // free the transaction
    transaction_destructor(t);
    putchar('|');
}

void constructor_test()
{
    Buffer from, to, privkey, date;
    buffer_constructor_str(&from, "Vincent", true);
    buffer_constructor_str(&to, "Jeanne", true);
    buffer_constructor_str(&privkey, "XXXX", true);
    buffer_constructor_str(&date, "04/05/2021", false);
    Transaction t;
    error = transaction_constructor(&t, from, to, 1.0, privkey);
    trans_test(t, from, to, privkey, date, 1.0, error, SUCCESS);
}

Test hash_tests[] = {
    {"constructor", constructor_test}};

void test_constructor()
{
    run_tests(hash_tests, sizeof(hash_tests) / sizeof(Test), 2);
}
