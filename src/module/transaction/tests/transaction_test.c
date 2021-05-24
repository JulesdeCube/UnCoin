#include "test_constructor.h"

Test tests[] = {
    {"TRANSACTION", test_constructor}};

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 1);
}