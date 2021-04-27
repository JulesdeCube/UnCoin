#include "../bigint.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                ARITHMETIQUE                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define BIGINT_OPERATION_GARD()                      \
    if (result == NULL)                              \
        return ERROR_VALUE;                          \
                                                     \
    if (bigint1 == NULL || bigint2 == NULL)          \
    {                                                \
        int error = bigint_constructor_null(result); \
        return error == SUCCESS ? NO_SELF : error;   \
    }

int _bigint_add_buffer_overflow(BigInt bigint1, BigInt bigint2, bool *overflow)
{
    // get exibitor and buffer size
    size_t exibitor_1 = bigint_get_exhibitor(bigint1);
    size_t exibitor_2 = bigint_get_exhibitor(bigint2);
    size_t size_1 = buffer_get_size(bigint1->buffer);
    size_t size_2 = buffer_get_size(bigint2->buffer);

    // get the new bigint min dimension
    size_t size_result = MAX(size_1, size_2);
    size_t exibitor_result = MAX(exibitor_1, exibitor_2);

    // if the exibitor is not the last bit ob the buffer no overflow is possible
    // we can use the next bit
    if (exibitor_result < size_result * sizeof(u_char))
    {
        *overflow = false;
        return SUCCESS;
    }

    // check if there not an overflow durring calculations
    u_char r = UCHAR_MAX;
    u_char byte1;
    u_char byte2;
    int error;
    // while we are not to the last byte and we are likely to overflow
    for (size_t i = 0; i < size_result && r == UCHAR_MAX; i++)
    {
        // get both byte
        error = bigint_get_byte(bigint1, i, &byte1);
        if (error != SUCCESS)
            return error;

        error = bigint_get_byte(bigint2, i, &byte2);
        if (error != SUCCESS)
            return error;

        // ceck if there is not an overflow
        if (__builtin_add_overflow(byte1, byte2, &r))
        {
            *overflow = true;
            return SUCCESS;
        }
    }

    // if there is no overflow no possible overflow and we check all the
    // addition
    *overflow = false;
    return SUCCESS;
}

int _bigint_add_size(BigInt bigint1, BigInt bigint2, size_t *size)
{
    size_t size1 = buffer_get_size(bigint1->buffer);
    size_t size2 = buffer_get_size(bigint2->buffer);

    *size = MAX(size1, size2);

    bool overflow;
    int error = _bigint_add_buffer_overflow(bigint1, bigint2, &overflow);
    if (error != SUCCESS)
        return error;

    *size += overflow == true;

    return SUCCESS;
}

int _bigint_add(BigInt bigint1, BigInt bigint2, BigInt *result)
{
    // check for pointer issus
    BIGINT_OPERATION_GARD();
    int error;

    // get the new number buffer size
    size_t size;
    error = _bigint_add_size(bigint1, bigint2, &size);
    if (error != SUCCESS)
        return error;

    // create result buffer
    Buffer buffer;
    error = buffer_constructor_const(&buffer, size, 0);
    if (error != SUCCESS)
        return error;

    u_char *p = buffer_get_data(buffer);

    // make the calcule
    bool overflow = false;
    u_char byte1;
    u_char byte2;
    for (size_t i = 0; i < size; i++, p++)
    {
        // get both byte
        error = bigint_get_byte(bigint1, i, &byte1);
        if (error != SUCCESS)
        {
            buffer_destructor(&buffer);
            return error;
        }

        error = bigint_get_byte(bigint2, i, &byte2);
        if (error != SUCCESS)
        {
            buffer_destructor(&buffer);
            return error;
        }

        // add the overflow and the first byte
        overflow = __builtin_add_overflow(overflow, byte1, p);
        // add the second byte
        overflow |= __builtin_add_overflow(*p, byte2, p);
    }

    // create the bigint
    error = bigint_constructor_buffer(result, POSITIVE, buffer);
    // delete the temp buffer
    buffer_destructor(&buffer);
    // exit with success
    return error;
}

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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 COMPARAISON                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int _bigint_comparison(BigInt bigint1, BigInt bigint2)
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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                GETTER/SETTER                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////