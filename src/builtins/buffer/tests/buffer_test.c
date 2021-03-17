#include "test/test.h"
#include "test_constructor.h"
#include "test_destructor.h"

Test tests[] = {
    {"CONSTRUCTOR", test_constructor},
    {"DESTRUCTOR", test_destructor}};

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 1);
}
