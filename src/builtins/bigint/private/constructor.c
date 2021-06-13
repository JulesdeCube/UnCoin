#include "../bigint.h"
#include <stdio.h>

size_t _bigint_get_array_exhibitor(size_t size, unsigned char *array)
{
    // get max exhibitor
    size_t exhibitor = size << 3;

    // find the first byte that is not null
    // and decrement the exhibitor each null byte
    for (; size; size--, exhibitor -= 8)
        // if the byte is not null break
        if (array[size - 1])
            break;

    // if we check every byte the array is null
    if (!size)
        // the exhibitor is equal to 0
        return 0;

    // find the first bit that is not null
    // and decremented the exhibtor each time
    for (unsigned char mask = 1 << 7; mask; mask >>= 1, --exhibitor)
        // if we find that the bit is positive break
        if (array[size - 1] & mask)
            break;

    // return the exhibitor
    return exhibitor;
}

size_t _bigint_get_buffer_exhibitor(Buffer buffer)
{
    return _bigint_get_array_exhibitor(buffer->size, buffer->data);
}

bool_t _bigint_is_valid_sign(bool_t sign)
{
    return sign == NEGATIVE || sign == POSITIVE;
}