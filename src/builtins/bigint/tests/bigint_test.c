#include "test/test.h"
#include "test_constructor.h"

Test tests[] = {
    {"CONSTRUCTOR", test_constructor}};

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 1);
    return 0;
}
