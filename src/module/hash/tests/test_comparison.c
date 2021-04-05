#include "test_comparison.h"

void hash_same_test(Buffer buffer1, Buffer buffer2, int error, int is_same)
{
    char *str1, *str2;
    size_t len1, len2;
    int r_error;

    // for the first buffer
    r_error = buffer_to_hex(buffer1, &str1, &len1);
    assert_equal_ul("error code", error, r_error);
    if (error != SUCCESS)
        assert_equal_p("to hex error null pointer (for the first)", NULL, str1);
    else
    {
        // for the second buffer
        r_error = buffer_to_hex(buffer2, &str2, &len2);
        assert_equal_ul("error code", error, r_error);
        if (error != SUCCESS)
            assert_equal_p("to hex error null pointer (for the second)", NULL, str2);
        else
        {
            // comparison if they are the same.
            void *s1 = str1;
            void *s2 = str2;
            size_t len = max_size_t(len1, len2);
            if(is_same)
                assert_equal_arr("are same", s1, s2, len);
            else
                assert_not_equal_arr("are different", s1, s2, len1);
        }
        free(str1);
        free(str2);
    }
    buffer_destructor(&buffer1);
    buffer_destructor(&buffer2);
    putchar('|');
}

void same_black_test()
{
    // create one
    Buffer buffer_result1;
    create_buff_hashed(&buffer_result1, "BlackHyptonique", 16);

    // create a second one
    Buffer buffer_result2;
    create_buff_hashed(&buffer_result2, "BlackHyptonique", 16);

    // test
    hash_same_test(buffer_result1, buffer_result2, SUCCESS, TRUE);
}

void diff_black_test()
{
    // create one
    Buffer buffer_result1;
    create_buff_hashed(&buffer_result1, "BlackHyptonique", 16);

    // create a second one
    Buffer buffer_result2;
    create_buff_hashed(&buffer_result2, "BlackHyptontque", 16);

    // test
    hash_same_test(buffer_result1, buffer_result2, SUCCESS, FALSE);
}

void same_point_test()
{
    // create one
    Buffer buffer_result1;
    create_buff_hashed(&buffer_result1, ".", 2);

    // create a second one
    Buffer buffer_result2;
    create_buff_hashed(&buffer_result2, ".", 2);

    // test
    hash_same_test(buffer_result1, buffer_result2, SUCCESS, TRUE);
}

void diff_point_test()
{
    // create one
    Buffer buffer_result1;
    create_buff_hashed(&buffer_result1, ".", 2);

    // create a second one
    Buffer buffer_result2;
    create_buff_hashed(&buffer_result2, ". ", 3);

    // test
    hash_same_test(buffer_result1, buffer_result2, SUCCESS, FALSE);
}

void diff_zero_test()
{
    // create two buffers
    Buffer buffer_result1;
    create_buff_hashed(&buffer_result1, "test\0", 6);
    Buffer buffer_result2;
    create_buff_hashed(&buffer_result2, "test\0\0", 7);
    // test
    hash_same_test(buffer_result1, buffer_result2, SUCCESS, FALSE);

    // create two buffers
    Buffer buffer_result11;
    create_buff_hashed(&buffer_result11, "te\0st", 6);
    Buffer buffer_result21;
    create_buff_hashed(&buffer_result21, "tes\0t", 6);
    // test
    hash_same_test(buffer_result11, buffer_result21, SUCCESS, FALSE);

}

Test hash_same_tests[] = {
    {"Same black", same_black_test},
    {"Different black", diff_black_test},
    {"Same point", same_point_test},
    {"Different point", diff_point_test},
    {"Different zero", diff_zero_test}
    };

void test_comparison()
{
    run_tests(hash_same_tests, sizeof(hash_same_tests) / sizeof(Test), 2);
}
