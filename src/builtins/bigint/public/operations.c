#include "../bigint.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                ARITHMETIQUE                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////
//              Private               //
////////////////////////////////////////

int _bigint_add_buffer_overflow(BigInt bigint1, BigInt bigint2, bool *overflow);

int _bigint_add_size(BigInt bigint1, BigInt bigint2, size_t *size);

int _bigint_add(BigInt bigint1, BigInt bigint2, BigInt *result);

////////////////////////////////////////
//               Public               //
////////////////////////////////////////

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

////////////////////////////////////////
//              Private               //
////////////////////////////////////////

#define BIGINT_TO(type)                                   \
    {                                                     \
        if (result == NULL)                               \
            return ERROR_VALUE;                           \
                                                          \
        *result = 0;                                      \
                                                          \
        if (bigint == NULL)                               \
            return NO_SELF;                               \
                                                          \
        if (bigint->buffer == NULL)                       \
            return INTERNAL_ERROR;                        \
                                                          \
        size_t size = buffer_get_size(bigint->buffer);    \
        u_char *data = buffer_get_data(bigint->buffer);   \
        u_char *pointer = (u_char *)result;               \
                                                          \
        if (!size)                                        \
            *result = 0;                                  \
        else if (size > sizeof(type))                     \
            *result = *(type *)data;                      \
        else                                              \
            memcpy(pointer, data, size);                  \
                                                          \
        if (bigint->sign == NEGATIVE)                     \
            *result = -*result;                           \
                                                          \
        return size <= sizeof(type) ? SUCCESS : NO_SPACE; \
    }
////////////////////////////////////////
//               Public               //
////////////////////////////////////////

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

    if (error != SUCCESS)
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

////////////////////////////////////////
//              Private               //
////////////////////////////////////////

/**
* @private
*
* get the sign of the substraction of bigint1 and bigint2
* @return 1 => bigin1 > bigint2
*         0 => bigin1 = bigint2
*        -1 => bigin1 < bigint2
*/
int _bigint_comparison(BigInt bigint1, BigInt bigint2);

////////////////////////////////////////
//               Public               //
////////////////////////////////////////

bool bigint_greater_than(BigInt bigint1, BigInt bigint2)
{
    return _bigint_comparison(bigint1, bigint2) == 1;
}

bool bigint_less_than(BigInt bigint1, BigInt bigint2)
{
    return _bigint_comparison(bigint1, bigint2) == -1;
}

bool bigint_equal_than(BigInt bigint1, BigInt bigint2)
{
    return _bigint_comparison(bigint1, bigint2) == 0;
}

bool bigint_not_equal(BigInt bigint1, BigInt bigint2)
{
    return _bigint_comparison(bigint1, bigint2) != 0;
}

bool bigint_less_or_equal(BigInt bigint1, BigInt bigint2)
{
    return _bigint_comparison(bigint1, bigint2) != 1;
}

bool bigint_greater_or_equal(BigInt bigint1, BigInt bigint2)
{
    return _bigint_comparison(bigint1, bigint2) != -1;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                GETTER/SETTER                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////
//               Private              //
////////////////////////////////////////

////////////////////////////////////////
//               Public               //
////////////////////////////////////////

size_t bigint_get_exhibitor(BigInt bigint)
{
    return (bigint != NULL) * bigint->exhibitor;
}

int bigint_get_byte(BigInt bigint, size_t i, u_char *byte)
{
    if (bigint == NULL)
        return NO_SELF;

    Buffer buffer = bigint->buffer;
    int error = buffer_get_index(buffer, i, byte);

    if (error == OUT_OF_RANGE)
    {
        *byte = 0;
        return SUCCESS;
    }

    return error;
}