#include "test_constructor.h"

int error;

void htab_test(Htab new_htab, int new_code, int error, size_t size, Destructor destructor)
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
        // free the htab
        htab_destructor(new_htab, destructor);
    }
    // add a htab test separator
    putchar('|');
}

void is_pair_test(Pair pair, Htab htab, int error, int is_same, Destructor destructor)
{
    int r_error;
    Pair new_pair;
    assert_equal_ul("error code", SUCCESS, error);
    if (error != SUCCESS)
        assert_equal_p("Htab pointer", NULL, htab);
    else
    {
        r_error = htab_get_corresponding_pair(htab, pair, &new_pair);
        assert_equal_ul("error corresponding", SUCCESS, r_error);
        // For each test :
        // - hash key
        // - key
        // - value
        if(is_same)
        {
            assert_equal_buffer("Comparison hash key",
                            pair->hkey,
                            new_pair->hkey);
            assert_equal_buffer("Comparison key",
                            pair->key,
                            new_pair->key);
            assert_equal_p("Comparison value",
                            pair->value,
                            new_pair->value);
        }
        else
        {
            // But don't forget a hash can be the same but
            // the keys can be different
            assert_not_equal_buffer("Comparison",
                                pair->key,
                                new_pair->key);
        }
        htab_destructor(htab, destructor);
    }
    putchar('|');
}

void construct_htab_from_array(Htab htab, size_t n_element, char **names, void **values)
{
    Buffer key;
    u_char *name;
    for(size_t i = 0; i < n_element; ++i)
    {
        name = (u_char *) names[i];
        buffer_constructor_array(&key, strlen(names[i]) + 1, name);
        htab_insert(htab, key, values[i]);
    }
}

void constructor_test()
{
    Htab htab;
    error = htab_constructor(&htab);
    htab_test(htab, error, SUCCESS, 0, NULL);
}

void pair_insert_test()
{
    Htab htab;
    htab_constructor(&htab);

    char *names[] = {"Turkey", "Jamaica", "Iraq", "Spain", "test", "CARAMBA"};
    void *values[] = {"Ankara", "Kingston", "Baghdad", "Madrid", NULL, NULL};
    construct_htab_from_array(htab, 6, names, values);

    print_htab(htab);

    htab_destructor(htab, NULL);
}

void pair_get_test()
{
    Htab htab;
    int error = htab_constructor(&htab);

    char *names[] = {"Turkey", "Jamaica", "Iraq", "Spain", "c zoli", "CARAMBA"};
    void *values[] = {"Ankara", "Kingston", "Baghdad", "Madrid", NULL, NULL};
    construct_htab_from_array(htab, 6, names, values);

    Buffer buf_key;
    char *str = "Iraq";
    buffer_constructor_array(&buf_key, strlen(str) + 1, (u_char *) str);
    Pair pair;
    Buffer buf_hkey;
    construct_pair(&pair, buf_key, "Baghdad", &buf_hkey);

    is_pair_test(pair, htab, error, true, NULL);
}

Test hash_tests[] = {
    {"constructor", constructor_test},
    {"pair insert", pair_insert_test},
    {"pair get", pair_get_test}};

void test_constructor()
{
    run_tests(hash_tests, sizeof(hash_tests) / sizeof(Test), 2);
}
