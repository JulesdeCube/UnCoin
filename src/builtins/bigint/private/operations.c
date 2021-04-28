#include "../bigint.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                GETTER/SETTER                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 COMPARAISON                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
int _bigint_unsigned_comparison(BigInt bigint1, BigInt bigint2)
{
    u_char *key1 = buffer_get_data(bigint1->buffer);
    u_char *key2 = buffer_get_data(bigint2->buffer);

    size_t size;
    if (buffer_get_size(bigint1->buffer) > buffer_get_size(bigint2->buffer))
        size = buffer_get_size(bigint1->buffer);
    else
    {
        size = buffer_get_size(bigint2->buffer);
    }
    for (; size; --size, key1++, key2++)
        if (*key1 != *key2)
            return (*key1 > *key2) ? 1 : -1;
    return 0;
}

int _bigint_comparison(BigInt bigint1, BigInt bigint2)
{
    BigInt x = bigint1;
    BigInt y = bigint2;
    if (x->sign != y->sign)
        return x->sign == POSITIVE ? 1 : -1;

    if (x->exhibitor != y->exhibitor)
        return (x->sign == POSITIVE) ^ (x->exhibitor > y->exhibitor) ? 1 : -1;

    int unsigned_res = _bigint_unsigned_comparison(bigint1, bigint2);
    return x->sign == POSITIVE ? unsigned_res : -unsigned_res;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   BITWISE                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                ARITHMETIQUE                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

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
    // get the new number buffer size
    size_t size;
    int error = _bigint_add_size(bigint1, bigint2, &size);
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

int _bigint_sub(BigInt bigint1, BigInt bigint2, BigInt *result)
{
    // get the new number buffer size
    size_t size1 = buffer_get_size(bigint1->buffer);
    size_t size2 = buffer_get_size(bigint2->buffer);
    size_t size = MAX(size1, size2);

    // create result buffer
    Buffer buffer;
    int error = buffer_constructor_const(&buffer, size, 0);
    if (error != SUCCESS)
        return error;

    u_char *p = buffer_get_data(buffer);

    // make the calcule
    bool carry = false;
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

        // substract the carry to the byte and check for carry
        carry = __builtin_sub_overflow(byte1, carry, p);
        // substract the byte
        carry |= __builtin_sub_overflow(*p, byte2, p);
    }

    // create the bigint
    error = bigint_constructor_buffer(result, POSITIVE, buffer);
    // delete the temp buffer
    buffer_destructor(&buffer);
    // exit with success
    return error;
}

int _bigint_add_sub(BigInt bigint1, BigInt bigint2, BigInt *result, bool sub)
{
    (void)sub;
    // if the first is null it's the second one
    if (bigint_is_null(bigint1))
        // if the 2 second is null to return null in the other case return the
        // second number
        return bigint_is_null(bigint2)
                   ? bigint_constructor_null(result)
                   : bigint_constructor_bigint(result, bigint2);

    // if the second element is null return a copy of the first one
    if (bigint_is_null(bigint2))
        return bigint_constructor_bigint(result, bigint1);

    // get bigint order
    int comparison = _bigint_unsigned_comparison(bigint1, bigint2);
    // check if the sign are different
    bool oposite_sign = bigint1->sign != bigint2->sign;

    // if the absolute value of the number are equal (|a| = |b|)
    if (comparison == 0)
    {
        // return null if there are oposite sign (a - a = 0)
        if (oposite_sign)
            return bigint_constructor_null(result);

        // if there are the same sign (a + a = 2a = a << 1)
        // TODO : use bitwise shift
        int error = _bigint_add(bigint1, bigint2, result);
        if (error == SUCCESS)
            (*result)->sign = bigint1->sign;
        return error;
    }

    // if the fist number is less that the second (a < b) invert it for the
    // substraction (need to have first number greater or equal to the second)
    if (comparison == -1)
    {
        BigInt tmp = bigint1;
        bigint1 = bigint2;
        bigint2 = tmp;
    }

    // make a substraction if there as oposite sign and a addition in the other
    // case
    int (*fn)(BigInt, BigInt, BigInt *) = oposite_sign ? _bigint_sub : _bigint_add;
    // apply the operation
    int error = fn(bigint1, bigint2, result);

    // the result sign is the oposite of the first one
    if (error == SUCCESS)
        (*result)->sign = bigint1->sign;

    return error;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 CONVERSION                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
