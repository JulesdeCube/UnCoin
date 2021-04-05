#include "test/test.h"
#include "test_comparison.h"
#include "test_hex.h"

Test tests[] = {
    {"HASH TO HEX", test_hex},
    {"HASH COMPARISON", test_comparison}};

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 1);
}