#include "../bigint.h"

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

bool bigint_is_null(BigInt bigint)
{
    return bigint_get_exhibitor(bigint) == 0;
}

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

int _bigint_unsigned_comparison(BigInt bigint1, BigInt bigint2);
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
//                                   BITWISE                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////
//              Private               //
////////////////////////////////////////

////////////////////////////////////////
//               Public               //
////////////////////////////////////////

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

int _bigint_sub(BigInt bigint1, BigInt bigint2, BigInt *result);

int _bigint_add_sub(BigInt bigint1, BigInt bigint2, BigInt *result, bool sub);

#define BIGINT_OPERATION_GARD()                      \
    if (result == NULL)                              \
        return ERROR_VALUE;                          \
                                                     \
    if (bigint1 == NULL || bigint2 == NULL)          \
    {                                                \
        int error = bigint_constructor_null(result); \
        return error == SUCCESS ? NO_SELF : error;   \
    }

////////////////////////////////////////
//               Public               //
////////////////////////////////////////

int bigint_addition(BigInt bigint1, BigInt bigint2, BigInt *result)
{
    // check for pointer issus
    BIGINT_OPERATION_GARD();
    return _bigint_add_sub(bigint1, bigint2, result, false);
}

int bigint_substraction(BigInt bigint1, BigInt bigint2, BigInt *result)
{
    // check for pointer issus
    BIGINT_OPERATION_GARD();
    return _bigint_add_sub(bigint1, bigint2, result, true);
}

int bigint_left_shift(BigInt bigint, size_t shift, BigInt *result)
{
    // GARD();

    if (shift == 0)
        return bigint_constructor_bigint(result, bigint);

    Buffer old_buffer = bigint->buffer;

    size_t old_exhibitor = bigint_get_exhibitor(bigint);
    size_t old_size = buffer_get_size(bigint->buffer);
    size_t new_exhibitor = old_exhibitor + shift;
    size_t new_size = new_exhibitor >> 3; // <=> / 8
    size_t current_shift = shift & 0x07;  // <=> % 8
    size_t last_shift = 8 - shift;

    if (current_shift)
        new_size++;

    Buffer new_buffer;
    TRY(buffer_constructor_const(&new_buffer, new_size, 0));

    u_char last_byte = 0x00;
    u_char current_byte;
    u_char new_byte;

    size_t old_i = 0;
    size_t new_i = shift >> 3; // <=> / 8
    for (; old_i < old_size; ++old_i, ++new_i)
    {
        TRY_CATCH(buffer_get_index(old_buffer, old_i, &current_byte),
                  buffer_destructor_safe(&new_buffer));

        new_byte = current_byte << current_shift | last_byte >> last_shift;
        printf("%lu -> %lu : %x\n", old_i, new_i, new_byte);

        TRY_CATCH(buffer_set_index(new_buffer,
                                   new_i,
                                   new_byte),
                  buffer_destructor_safe(&new_buffer));

        last_byte = current_byte;
    }

    return bigint_constructor_buffer(result, bigint->sign, new_buffer);
}

int bigint_right_shift(BigInt bigint, size_t shift, BigInt *result)
{
    if (shift == 0)
        return bigint_constructor_bigint(result, bigint);

    size_t exhibitor = bigint_get_exhibitor(bigint);

    if (shift >= exhibitor)
        return bigint_constructor_null(result);

    return true;
}

int bigint_shift(BigInt bigint, ssize_t shift, BigInt *result)
{
    return shift > 0
               ? bigint_left_shift(bigint, shift, result)
               : bigint_right_shift(bigint, -shift, result);
}

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

    *result = !bigint_is_null(bigint);

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

    TRY(buffer_to_hex(bigint->buffer, str, len));

    if (len)
        *len += 2 + (bigint->sign == NEGATIVE);

    if (str == NULL)
        return SUCCESS;

    char *buffer_str = *str;
    int error = asprintf(str,
                         "%s0x%s",
                         bigint->sign == NEGATIVE ? "-" : "",
                         buffer_str);
    free(buffer_str);

    return error == -1 ? NO_SPACE : SUCCESS;
}
