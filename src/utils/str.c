#include "str.h"

bool_t is_letter(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

bool_t ascii_char(char c)
{
    return c >= ' ' && c <= '~';
}

size_t string_len(string_t str, bool_t allow_esc)
{
    size_t len = 0;
    for (; *str; ++str)
    {
        if (allow_esc && *str == '\033')
            while (*str && !is_letter(*str))
                str++;
        else
            len++;
    }
    return len;
}

bool_t is_number(char c)
{
    return c >= '0' && c <= '9';
}

bool_t str_contain(string_t str, char c)
{
    while (*str && *str != c)
        str++;
    return *str == c;
}

bool_t float_validator(string_t str, char c)
{
    return is_number(c) || (c == '.' && !str_contain(str, c));
}

bool_t unsigned_validator(string_t str, char c)
{
    (void)str;
    return is_number(c);
}

error_t sub_str(string_t str, size_t size, string_t *result)
{
    // check if we can send a result
    if (result == NULL)
        return ERROR_VALUE;

    // if an error ocure the result is null
    *result = NULL;

    // if there is no string resurn an error
    if (str == NULL)
        return NO_SELF;

    // alocate the string + the null endding
    *result = calloc(size + 1, sizeof(char));

    // copy the string
    memcpy(*result, str, size);

    return SUCCESS;
}

error_t split_lines(string_t str, Array *result)
{
    if (str == NULL)
        return NO_SELF;

    if (result == NULL)
        return ERROR_VALUE;

    TRY(array_constructor(result, sizeof(string_t)));

    string_t split;
    string_t s = str;
    for (; *s; s++)
    {
        if (*s != '\n')
            continue;

        TRY_CATCH(sub_str(str, s - str, &split),
                  array_destructor(result, free));

        TRY_CATCH(array_push(*result, &split),
                  array_destructor(result, free));

        str = s + 1;

        if (s[1])
            s++;
    }

    TRY_CATCH(sub_str(str, s - str, &split),
              array_destructor(result, free));

    TRY_CATCH(array_push(*result, &split),
              array_destructor(result, free));

    return SUCCESS;
}
