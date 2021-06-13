#include "test.h"

void assert_equal_bigint(string_t title, BigInt expected, BigInt got)
{
    string_t title_value, exp_str, got_str;

    if (asprintf(&title_value, "%s: value", title) == -1)
        errx(1, "cant create bigint title value test");

    int error = bigint_to_string(expected, &exp_str, NULL);
    if (error != SUCCESS)
        errx(1, "can't create bigint string value of exprected : %i", error);

    error = bigint_to_string(got, &got_str, NULL);
    if (error != SUCCESS)
        errx(1, "can't create bigint string value of got : %i", error);

    assert_equal_s(title_value, exp_str, got_str);

    free(exp_str);
    free(got_str);
    free(title_value);

    string_t title_exib;

    if (asprintf(&title_exib, "%s: exhibitor", title) == -1)
        errx(1, "cant create bigint title exhibitor test");
    assert_equal_ul(title_exib,
                    bigint_get_exhibitor(expected),
                    bigint_get_exhibitor(got));
    free(title_exib);
}