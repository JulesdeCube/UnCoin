#include "../bigint.h"

#define BIGINT_TO(type)                                          \
    {                                                            \
        if (bigint == NULL)                                      \
            return NO_SELF;                                      \
                                                                 \
        if (bigint->buffer == NULL)                              \
            return INTERNAL_ERROR;                               \
                                                                 \
        if (result == NULL)                                      \
            return ERROR_VALUE;                                  \
                                                                 \
        size_t size = buffer_get_size(bigint->buffer);           \
        u_char *data = buffer_get_data(bigint->buffer);          \
        u_char *pointer = (u_char *)result;                      \
                                                                 \
        if (!size)                                               \
            return 0;                                            \
        else if (size > sizeof(type))                            \
            *result = *(type *)data;                             \
        else                                                     \
            memcpy(pointer - (sizeof(type) - size), data, size); \
                                                                 \
        if (bigint->sign == NEGATIVE)                            \
            *result = -*result;                                  \
                                                                 \
        return SUCCESS;                                          \
    }

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                ARITHMETIQUE                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   BITWISE                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 CONVERSION                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int bigint_to_bool(BigInt bigint, bool *result)
{
    if (bigint == NULL)
        return NO_SELF;

    if (result == NULL)
        return ERROR_VALUE;

    *result = bigint->exhibitor != 0;

    return SUCCESS;
}

int bigint_to_int(BigInt bigint, int *result)
    BIGINT_TO(int);

long long int bigint_to_long_long_int(BigInt bigint, long long int *result)
    BIGINT_TO(long long int);

int bigint_to_buffer(BigInt bigint, Buffer *buffer)
{

    if (buffer == NULL)
        return ERROR_VALUE;

    *buffer = NULL;

    if (bigint == NULL)
        return NO_SELF;

    if (bigint->buffer == NULL)
        return INTERNAL_ERROR;

    return buffer_constructor_buffer(buffer, bigint->buffer);
}

int bigint_to_string(BigInt bigint, char **str, size_t *len)
{
    if (bigint == NULL)
        return NO_SELF;

    if (bigint->buffer == NULL)
        return INTERNAL_ERROR;

    int error = buffer_to_hex(bigint->buffer, str, len);

    if (error == SUCCESS)
        return error;

    if (len)
        *len += 2 + (bigint->sign == NEGATIVE);

    if (str == NULL)
        return SUCCESS;

    char *buffer_str = *str;
    int print_error = asprintf(str,
                               "%s0x%s",
                               bigint->sign == NEGATIVE ? "-" : "",
                               buffer_str);
    free(buffer_str);

    return print_error == -1 ? NO_SPACE : SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 COMPARAISON                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//return  1 if BG1 > BG2, 0 if BG1 == BG2, -1 if BG1 < BG2
int _bigint_comparison(BigInt bigint1,BigInt bigint2)
{
    BigInt x = bigint1;
    BigInt y = bigint2;
    if (x->sign != y->sign)
        return x->sign == POSITIVE ? 1 : -1;

    if (x->exhibitor != y->exhibitor)
        return (x->sign == POSITIVE) ^ (x->exhibitor > y->exhibitor) ? 1 : -1;

    u_char *key1 = buffer_get_data(x->buffer);
    u_char *key2 = buffer_get_data(y->buffer);
    size_t size = buffer_get_size(x->buffer);

    for (; size; --size, key1++, key2++)
        if (*key1 != *key2)
            return *key1 > *key2 ? 1 : -1;

    return 0;
}

bool bigint_greater_than(BigInt bigint1,BigInt bigint2)
{
    if(_bigint_comparison(bigint1,bigint2) == 1)
        return true;
    return false;
}

bool bigint_less_than(BigInt bigint1,BigInt bigint2)
{
   if(_bigint_comparison(bigint1,bigint2) == -1)
        return true;
    return false;
}

bool bigint_equal_than(BigInt bigint1,BigInt bigint2)
{
    if(_bigint_comparison(bigint1,bigint2) == 0)
        return true;
    return false;
}

bool bigint_not_equal(BigInt bigint1,BigInt bigint2)
{
    if(_bigint_comparison(bigint1,bigint2) != 0)
        return true;
    return false;
}

bool bigint_less_or_equal(BigInt bigint1,BigInt bigint2)
{
   if(_bigint_comparison(bigint1,bigint2) != 1)
        return true;
    return false;
}

bool bigint_greater_or_equal(BigInt bigint1,BigInt bigint2)
{
    if(_bigint_comparison(bigint1,bigint2) != -1)
        return true;
    return false;
}