#include "test_hex.h"

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
    create_buff_hashed(&bh_result1, "BlackHyptonique", 16);
    hash_hex_test(bh_result1, SUCCESS, "1ef4a9088e140285");

    // test BlackHyptontque
    Buffer bh_result2;
    create_buff_hashed(&bh_result2, "BlackHyptontque", 16);
    hash_hex_test(bh_result2, SUCCESS, "c4fafd2ffe70c0c9");
}

void from_tp_test()
{
    // test France
    Buffer france;
    create_buff_hashed(&france, "France", 7);
    hash_hex_test(france, SUCCESS, "78029c9243ea950f");
}

void with_zero_test()
{
    // test with a zero

    Buffer buff;
    create_buff_hashed(&buff, "\0France", 8);
    hash_hex_test(buff, SUCCESS, "78029c9243ea950f");

    Buffer buff2;
    create_buff_hashed(&buff2, "Fra\0nce", 8);
    hash_hex_test(buff2, SUCCESS, "005327589bcfcf8f");

    Buffer buff3;
    create_buff_hashed(&buff3, "France\0\0\0\0\0\0", 8);
    hash_hex_test(buff3, SUCCESS, "d62b1e054c5bd13a");
}

Test hash_tests[] = {
    {"blackhyptonique", black_test},
    {"from tp", from_tp_test},
    {"with zero", with_zero_test}
    };

void test_hex()
{
    run_tests(hash_tests, sizeof(hash_tests) / sizeof(Test), 2);
}
