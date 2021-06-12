#include "test_constructor.h"

int error, r_error;

void trans_test(Transaction t, Buffer from, Buffer to,
  long long unsigned int amount, time_t time,
  int error, int new_code)
{
    // check the error code
    assert_equal_ul("Error code", error, new_code);

    assert_equal_buffer("From", from, t->from);
    assert_equal_buffer("To", to, t->to);

    assert_equal_ul("Amount", (size_t) amount, (size_t) t->amount);

    (void) time;

    // free the transaction
    transaction_destructor(t);

    // free buffers
    buffer_destructor_safe(&from);
    buffer_destructor_safe(&to);

    putchar('|');
}

void constructor_test()
{
    Buffer from, from2, to, to2;
    buffer_constructor_str(&from, "Vincent", true);
    buffer_constructor_str(&to, "Jeanne", true);

    Transaction tra;
    buffer_constructor_str(&from2, "Vincent", true);
    buffer_constructor_str(&to2, "Jeanne", true);
    // Get current time
    time_t t;
    time(&t);
    error = transaction_constructor_client(&tra, from2, to2, 1.0, t);
    trans_test(tra, from, to, 1.0, t, error, SUCCESS);
}

void to_buffer_test()
{
    Buffer from, to, result;
    Transaction tra;
    buffer_constructor_str(&from, "Vincent", true);
    buffer_constructor_str(&to, "Jeanne", true);
    time_t t;
    time(&t);
    transaction_constructor_client(&tra, from, to, 1.0, t);
    transaction_to_buffer(tra, &result);
    error = transaction_from_buffer(&tra, &result);
    trans_test(tra, from, to, 1.0, t, error, SUCCESS);

    buffer_constructor_str(&from, "Baptiste", true);
    buffer_constructor_str(&to, "Jules", true);
    transaction_constructor_client(&tra, from, to, 1487181, t);
    transaction_to_buffer(tra, &result);
    error = transaction_from_buffer(&tra, &result);
    trans_test(tra, from, to, 1487181, t, error, SUCCESS);
}

Test hash_tests[] = {
    {"constructor", constructor_test},
    {"to buffer", to_buffer_test}};

void test_constructor()
{
    run_tests(hash_tests, sizeof(hash_tests) / sizeof(Test), 2);
}
