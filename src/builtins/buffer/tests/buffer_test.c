#include "test.h"

void test_constructor();
void test_destructor();

Test tests[] = {
    {"CONSTRUCTOR", test_constructor},
    {"DESTRUCTOR", test_destructor}};

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 0);
}
