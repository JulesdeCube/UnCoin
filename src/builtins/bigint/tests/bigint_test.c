#include "test/test.h"
#include "test_constructor.h"
#include "test_conversion.h"

Test tests[] = {
    {"CONSTRUCTOR", test_constructor},
    {"CONVERSION", test_conversion}};

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 1);
    return 0;
}
