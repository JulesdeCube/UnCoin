#include "test/test.h"
#include "test_constructor.h"
#include "test_destructor.h"
#include "test_getter.h"
#include "test_operator.h"

Test tests[] = {
    {"CONSTRUCTOR", test_constructor},
    {"DESTRUCTOR", test_destructor},
    {"GETTER", test_getter},
    {"OPERATOR", test_operator}};

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 1);
}
