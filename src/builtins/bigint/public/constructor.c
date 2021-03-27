#include "../bigint.h"

/**
* @private
*
* find the exhibtor of a not normalize array
*/
size_t _bigint_get_array_exhibitor(size_t size, unsigned char *array)
{
    // get max exhibitor
    size_t exhibitor = size << 3;

    // find the first byte that is not null
    // and decrement the exhibitor each null byte
    size_t i = 0;
    for (; i < size; i++, exhibitor -= 8)
        // if the byte is not null break
        if (array[i])
            break;

    // if we check every byte the array is null
    if (i >= size)
        // the exhibitor is equal to 0
        return 0;

    // find the fist bit that is not null
    // and decremented the exhibtor each time
    for (unsigned char mask = 1 << 7; mask; mask >>= 1, --exhibitor)
        // if we find that the bit is positive break
        if (array[i] & mask)
            break;

    // return the exhibitor
    return exhibitor;
}

size_t _bigint_get_buffer_exhibitor(Buffer buffer)
{
    return _bigint_get_array_exhibitor(buffer->size, buffer->data);
}

int bigint_constructor_null(BigInt *new_bigint)
{
    BigInt bigint = malloc(sizeof(struct s_bigint));

    if (bigint == NULL)
        return NO_SPACE;

    bigint->exhibitor = 0;
    bigint->sign = POSITIVE;

    int error = buffer_constructor_size(&bigint->buffer, 0);
    if (error != SUCCESS)
    {
        bigint_destructor(&bigint);
        return error;
    }

    *new_bigint = bigint;
    return SUCCESS;
}

int bigint_constructor_array(BigInt *new_bigint, bool sign, size_t size, unsigned char *array)
{
    *new_bigint = NULL;

    // !TODO - create a sub function to check if is a valid sign
    if (sign != NEGATIVE && sign != POSITIVE)
        return ERROR_VALUE;

    if (array == NULL)
        return NO_SELF;

    BigInt bigint = malloc(sizeof(struct s_bigint));

    if (bigint == NULL)
        return NO_SPACE;

    Buffer buffer;
    int error = buffer_constructor_array(&buffer, size, array);
    if (error != SUCCESS)
    {
        bigint_destructor(&bigint);
        return error;
    }

    bigint->buffer = buffer;
    bigint->exhibitor = _bigint_get_buffer_exhibitor(buffer);
    bigint->sign = sign;

    *new_bigint = bigint;

    return SUCCESS;
}