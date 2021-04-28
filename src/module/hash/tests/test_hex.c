#include "test_hex.h"

Buffer buffer;

void hash_hex_test(Buffer buffer, int error, char *expected)
{
    char *str;
    size_t len;
    int r_error = buffer_to_hex(buffer, &str, &len);

    assert_equal_ul("error code", error, r_error);

    if (error == SUCCESS)
    {
        assert_equal_s("value error", expected, str);
        assert_equal_ul("lenght error", len, strlen(str));

        free(str);
    }
    else
        assert_equal_p("to hex error null pointer", NULL, str);

    buffer_destructor(&buffer);
    putchar('|');
}

void black_test()
{
    // test BlackHyptonique
    Buffer bh_result1;
    create_buff_hashed(&bh_result1, "BlackHyptonique");
    hash_hex_test(bh_result1, SUCCESS, "b6b80c6b8ce56af9");

    // test BlackHyptontque
    Buffer bh_result2;
    create_buff_hashed(&bh_result2, "BlackHyptontque");
    hash_hex_test(bh_result2, SUCCESS, "c10ca36584e351b1");
}

void from_tp_test()
{
    // test France
    Buffer france;
    create_buff_hashed(&france, "France");
    hash_hex_test(france, SUCCESS, "d5cc0146333e695b");
}

Test hash_tests[] = {
    {"blackhyptonique", black_test},
    {"from tp", from_tp_test}};

void test_hex()
{
    run_tests(hash_tests, sizeof(hash_tests) / sizeof(Test), 2);
}
