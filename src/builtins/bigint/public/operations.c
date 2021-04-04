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