#include "../bigint.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  PRIVATE                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/**
* @private
*
* find the exhibtor of a not normalize array
*/
size_t _bigint_get_array_exhibitor(size_t size, unsigned char *array);

/**
* @private
*
* find the exhibtor of a not normalize buffer
*/
size_t _bigint_get_buffer_exhibitor(Buffer buffer);

/**
* @private
*
* check if the sign is valid
*/
bool _bigint_is_valid_sign(bool sign);

int bigint_constructor_null(BigInt *new_bigint)
{
    u_char array[] = {0x00};
    return bigint_constructor_array(new_bigint, POSITIVE, 0, array);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   PUBLIC                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
int bigint_constructor_array(BigInt *new_bigint, bool sign, size_t size,
                             u_char *array)
{

    // if there is
    if (new_bigint == NULL)
        return NO_SELF;

    *new_bigint = NULL;

    if (!_bigint_is_valid_sign(sign))
        return ERROR_VALUE;

    if (array == NULL)
        return ERROR_VALUE;

    BigInt bigint = malloc(sizeof(struct s_bigint));

    if (bigint == NULL)
        return NO_SPACE;

    bigint->exhibitor = _bigint_get_array_exhibitor(size, array);
    size_t normalize_size = bigint->exhibitor >> 3;

    if (bigint->exhibitor & 0x07)
        ++normalize_size;

    bigint->sign = bigint->exhibitor ? sign : POSITIVE;

    int error = buffer_constructor_array(&bigint->buffer,
                                         normalize_size,
                                         array);
    if (error != SUCCESS)
    {
        bigint_destructor(&bigint);
        return error;
    }

    *new_bigint = bigint;

    return SUCCESS;
}

int bigint_constructor_buffer(BigInt *new_bigint, bool sign, Buffer buffer)
{

    return bigint_constructor_array(new_bigint,
                                    sign,
                                    buffer_get_size(buffer),
                                    buffer_get_data(buffer));
}

int bigint_constructor_buffer_signed(BigInt *new_bigint, Buffer buffer)
{
    // check if we have the big int
    if (new_bigint == NULL)
        return NO_SELF;

    // set to null in case of error
    *new_bigint = NULL;

    // if there is no buffer
    if (buffer == NULL)
        return ERROR_VALUE;

    // get the more significative bit
    u_char *more_byte = buffer_get_data(buffer);
    // get the sign (last bit of the first buffer byte the bit in for left of
    // the memory)
    bool sign = (*more_byte & 0x80) ? NEGATIVE : POSITIVE;

    // set byte to 0 (for the minimal size)
    *more_byte &= 0x7f;

    // create the buffer
    int error = bigint_constructor_array(new_bigint,
                                         sign,
                                         buffer_get_size(buffer),
                                         buffer_get_data(buffer));

    // set byte to the previous value
    *more_byte |= (sign == NEGATIVE) << 7;

    return error;
}

int bigint_constructor_bigint(BigInt *new_bigint, BigInt bigint)
{
    // if there is no bigint
    if (bigint == NULL)
        return ERROR_VALUE;

    // set to null in case of error
    *new_bigint = NULL;

    // if there is no buffer returtn an error
    if (bigint->buffer == NULL)
        return INTERNAL_ERROR;

    // create the buffer
    return bigint_constructor_buffer(new_bigint,
                                     bigint->sign,
                                     bigint->buffer);
}

int bigint_constructor_from_int(BigInt *new_bigint, int value)
{
    bool sign = value < 0 ? NEGATIVE : POSITIVE;
    value = abs(value);
    u_char *array = (u_char *)&value;
    return bigint_constructor_array(new_bigint, sign, sizeof(int), array);
}
