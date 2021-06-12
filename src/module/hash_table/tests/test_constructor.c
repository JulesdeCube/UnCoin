#include "test_constructor.h"

int error, r_error;
Htab htab;
Buffer buf_key;
char *names[] = {"Turkey", "Jamaica", "Iraq", "Spain", "c zoli", "CARAMBA", "test"};
void *values[] = {"Ankara", "Kingston", "Baghdad", "Madrid", NULL, NULL, NULL};

void htab_test(Htab new_htab, int new_code, int error, size_t size, size_t capacity,
               Destructor destructor)
{
    // check the error code
    assert_equal_ul("Error code", error, new_code);
    // if it's not a success check if the htab is correcly set
    if (error != SUCCESS)
        assert_equal_p("Htab pointer", NULL, new_htab);
    // if all wen't good do the htab check
    else
    {
        // test the size
        assert_equal_ul("Size", size, new_htab->size);
        // test the capacity
        assert_equal_ul("Capacity", capacity, new_htab->capacity);
    }
    // free the htab
    htab_destructor(new_htab, destructor);

    putchar('|');
}

void is_pair_test(Buffer key, void *value, Htab htab, int error, int is_same, Destructor destructor)
{
    assert_equal_ul("error code", SUCCESS, error);
    if (error != SUCCESS)
        assert_equal_p("Htab pointer", NULL, htab);
    else
    {
        void *res_value;
        r_error = htab_get(htab, key, &res_value);
        assert_equal_ul("error corresponding", SUCCESS, r_error);
        if (is_same)
        {
            assert_equal_p("Comparison values",
                           value,
                           res_value);
        }
        else
        {
            assert_not_equal_p("Comparison values",
                               value,
                               res_value);
        }
        if (destructor != NULL)
            destructor(res_value);
    }

    htab_destructor(htab, destructor);

    if (destructor != NULL)
        destructor(value);

    buffer_destructor_safe(&key);
    putchar('|');
}

void htab_remove_list(Htab htab, char *str_list[], size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        buffer_constructor_array(&buf_key, strlen(str_list[i]) + 1, (u_char *)str_list[i]);
        error = htab_remove(htab, buf_key, NULL);
        if (error != SUCCESS)
            printf("ERROR : %d\n", error);
    }
}

void htab_pop_list(Htab htab, char *str_list[], void *values[], size_t n)
{
    void *value;
    for (size_t i = 0; i < n; ++i)
    {
        buffer_constructor_array(&buf_key, strlen(str_list[i]) + 1, (u_char *)str_list[i]);
        error = htab_pop(htab, buf_key, &value, NULL);
        if (error != SUCCESS)
            printf("ERROR : %d\n", error);
        assert_equal_p("Values", values[i], value);
    }
}

void constructor_test()
{
    error = htab_constructor(&htab);
    htab_test(htab, error, SUCCESS, 0, 4, NULL);
}

void pair_insert_test()
{
    htab_constructor(&htab);
    construct_htab_from_array(htab, 4, names, values);

    htab_test(htab, error, SUCCESS, 4, 8, NULL);
}

void pair_get_test()
{
    htab_constructor(&htab);
    construct_htab_from_array(htab, 7, names, values);

    error = buffer_constructor_str(&buf_key, "Iraq", true);

    is_pair_test(buf_key, "Baghdad", htab, error, true, NULL);
}

void pair_remove_test()
{
    htab_constructor(&htab);

    construct_htab_from_array(htab, 4, names, values);

    // To remove all of this
    char *str_list[] = {"Spain", "Turkey", "Iraq", "Jamaica"};
    htab_remove_list(htab, str_list, 4);

    htab_test(htab, error, SUCCESS, 0, 4, NULL);
}

void pair_pop_test()
{
    htab_constructor(&htab);

    construct_htab_from_array(htab, 4, names, values);

    // To remove all of this
    char *str_list[] = {"Spain", "Turkey", "Iraq", "Jamaica"};
    void *values[] = {"Madrid", "Ankara", "Baghdad", "Kingston"};
    htab_pop_list(htab, str_list, values, 4);

    htab_test(htab, error, SUCCESS, 0, 4, NULL);
}

Test hash_tests[] = {
    {"constructor", constructor_test},
    {"pair insert", pair_insert_test},
    {"pair get", pair_get_test},
    {"pair remove", pair_remove_test},
    {"pair pop", pair_pop_test}};

void test_constructor()
{
    run_tests(hash_tests, sizeof(hash_tests) / sizeof(Test), 2);
}
