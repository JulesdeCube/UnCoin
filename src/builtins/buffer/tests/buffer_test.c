#include "test/test.h"
#include "test_constructor.h"
#include "test_destructor.h"
#include "test_getter.h"

Test tests[] = {
    {"CONSTRUCTOR", test_constructor},
    {"DESTRUCTOR", test_destructor},
    {"GETTER", test_getter}};

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 1);
}
