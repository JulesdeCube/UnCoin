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
    size_t exhibitor_1 = bigint_get_exhibitor(bigint1);
    size_t exhibitor_2 = bigint_get_exhibitor(bigint2);

    if (bigint1->exhibitor != bigint2->exhibitor)
        return exhibitor_1 > exhibitor_2 ? 1 : -1;

    u_char *key1 = buffer_get_data(bigint1->buffer);
    u_char *key2 = buffer_get_data(bigint2->buffer);

    size_t size = buffer_get_size(bigint1->buffer);
    for (; size; --size, key1++, key2++)
        if (*key1 != *key2)
            return (*key1 > *key2) ? 1 : -1;

    return 0;
}

int _bigint_comparison(BigInt bigint1, BigInt bigint2)
{
    if (bigint1->sign != bigint2->sign)
        return bigint1->sign == POSITIVE ? 1 : -1;

    int unsigned_res = _bigint_unsigned_comparison(bigint1, bigint2);

    if (bigint1->sign == NEGATIVE)
        unsigned_res = -unsigned_res;

    return unsigned_res;
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

error_t _bigint_byte_operation(BigInt bigint1, BigInt bigint2, BigInt *result,
                           size_t new_size,
                           u_char (*byte_operator)(u_char, u_char, bool *))
{
    // create result buffer
    Buffer buffer;
    TRY(buffer_constructor_const(&buffer, new_size, 0));

    // make the calcule
    bool tmp = false;
    u_char new_byte, byte1, byte2;
    for (size_t i = 0; i < new_size; i++)
    {
        // get both byte
        TRY_CATCH(bigint_get_byte(bigint1, i, &byte1),
                  buffer_destructor(&buffer));

        TRY_CATCH(bigint_get_byte(bigint2, i, &byte2),
                  buffer_destructor(&buffer));

        // apply the operation
        new_byte = byte_operator(byte1, byte2, &tmp);

        // change the valeu
        TRY_CATCH(buffer_set_index(buffer, i, new_byte),
                  buffer_destructor(&buffer));
    }

    // create the bigint
    TRY_CATCH(bigint_constructor_buffer(result, POSITIVE, buffer),
              buffer_destructor(&buffer));

    // delete the temp buffer
    buffer_destructor(&buffer);
    // exit with success
    return SUCCESS;
}

u_char _bigint_sub_byte(u_char byte1, u_char byte2, bool *carry)
{
    u_char new_byte = 0;

    // substract the carry
    *carry = __builtin_sub_overflow(byte1, *carry, &new_byte);
    // substract the byte
    *carry |= __builtin_sub_overflow(new_byte, byte2, &new_byte);

    return new_byte;
}

u_char _bigint_add_byte(u_char byte1, u_char byte2, bool *overflow)
{
    u_char new_byte = 0;

    // add the overflow and the first byte
    *overflow = __builtin_add_overflow(*overflow, byte1, &new_byte);
    // add the second byte
    *overflow |= __builtin_add_overflow(new_byte, byte2, &new_byte);

    return new_byte;
}

error_t _bigint_add_buffer_overflow(BigInt bigint1, BigInt bigint2, bool *overflow)
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
    if (exibitor_result < size_result << 3)
    {
        *overflow = false;
        return SUCCESS;
    }

    // check if there not an overflow durring calculations
    u_char r = UCHAR_MAX;
    u_char byte1;
    u_char byte2;
    // while we are not to the last byte and we are likely to overflow
    for (; size_result && r == UCHAR_MAX; --size_result)
    {
        // get both byte
        TRY(bigint_get_byte(bigint1, size_result - 1, &byte1));

        TRY(bigint_get_byte(bigint2, size_result - 1, &byte2))

        // check if there is not an overflow
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

error_t _bigint_add_size(BigInt bigint1, BigInt bigint2, size_t *size)
{
    size_t size1 = buffer_get_size(bigint1->buffer);
    size_t size2 = buffer_get_size(bigint2->buffer);

    *size = MAX(size1, size2);

    bool overflow = true;
    // TRY(_bigint_add_buffer_overflow(bigint1, bigint2, &overflow));

    *size += overflow == true;

    return SUCCESS;
}

error_t _bigint_add(BigInt bigint1, BigInt bigint2, BigInt *result)
{
    // get the new number buffer size
    size_t size1 = buffer_get_size(bigint1->buffer);
    size_t size2 = buffer_get_size(bigint2->buffer);
    size_t size = MAX(size1, size2) + 1;

    return _bigint_byte_operation(bigint1, bigint2, result, size, _bigint_add_byte);
}

error_t _bigint_sub(BigInt bigint1, BigInt bigint2, BigInt *result)
{
    // get the new number buffer size
    size_t size1 = buffer_get_size(bigint1->buffer);
    size_t size2 = buffer_get_size(bigint2->buffer);
    size_t size = MAX(size1, size2);

    return _bigint_byte_operation(bigint1, bigint2, result, size, _bigint_sub_byte);
}

error_t _bigint_add_sub(BigInt bigint1, BigInt bigint2, BigInt *result, bool sub)
{
    // if the first is null it's the second one
    if (bigint_is_null(bigint1))
    {
        // if the 2 second is null to return null in the other case return the
        // second number
        if (bigint_is_null(bigint2))
            return bigint_constructor_null(result);

        TRY(bigint_constructor_bigint(result, bigint2));

        (*result)->sign = (*result)->sign ^ sub;

        return SUCCESS;
    }

    // if the second element is null return a copy of the first one
    if (bigint_is_null(bigint2))
        return bigint_constructor_bigint(result, bigint1);

    // get bigint order
    int comparison = _bigint_unsigned_comparison(bigint1, bigint2);
    // check if the sign are different
    bool oposite_sign = (bigint1->sign != bigint2->sign) ^ sub;

    // if the absolute value of the number are equal (|a| = |b|)
    if (comparison == 0)
    {
        // return null if there are oposite sign (a - a = 0)
        if (oposite_sign)
            return bigint_constructor_null(result);

        // if there are the same sign (a + a = 2a = a << 1)
        // TODO : use bitwise shift
        TRY(_bigint_add(bigint1, bigint2, result));

        // use the proper sign
        (*result)->sign = bigint1->sign;
        return SUCCESS;
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
    TRY(fn(bigint1, bigint2, result));

    // the result sign is the same of the first number
    (*result)->sign = bigint1->sign ^ (sub && (comparison == -1));

    return SUCCESS;
}

error_t _bigint_count_bit(BigInt bigint, size_t *result)
{
    if (bigint == NULL)
        return NO_SELF;

    if (result == NULL)
        return ERROR_VALUE;

    if (bigint->buffer == NULL)
        return INTERNAL_ERROR;

    size_t size = buffer_get_size(bigint->buffer);
    u_char *d = buffer_get_data(bigint->buffer);

    *result = 0;

    for (; size; --size, ++d)
        for (u_char mask = 1; mask; mask <<= 1)
            *result += !!(*d & mask);

    return SUCCESS;
}

error_t _bigint_mul(BigInt bigint1, BigInt bigint2, BigInt *result)
{
    BigInt tmp, bigint_shift;
    size_t nb_bit1, nb_bit2;
    TRY(_bigint_count_bit(bigint1, &nb_bit1));
    TRY(_bigint_count_bit(bigint2, &nb_bit2));

    if (nb_bit1 > nb_bit2)
    {
        tmp = bigint1;
        bigint1 = bigint2;
        bigint2 = tmp;
    }

    size_t size = buffer_get_size(bigint1->buffer);
    u_char *d = buffer_get_data(bigint1->buffer);

    TRY(bigint_constructor_null(result));

    size_t bit = 0;
    for (; size; --size, ++d)
        for (u_char mask = 1; mask; mask <<= 1, ++bit)
            if (*d & mask)
            {
                tmp = *result;
                TRY_CATCH(bigint_left_shift(bigint2, bit, &bigint_shift),
                          bigint_destructor(result));
                TRY_CATCH(_bigint_add(*result, bigint_shift, result),
                          {
                              bigint_destructor(&bigint_shift);
                              bigint_destructor(result);
                          });
                char *res_hex, *shift_hex, *tmp_hex;
                TRY(bigint_to_string(*result, &res_hex, NULL));
                TRY(bigint_to_string(bigint_shift, &shift_hex, NULL));
                TRY(bigint_to_string(tmp, &tmp_hex, NULL));
                // printf("print(hex(%s + %s));", tmp_hex, shift_hex);
                // printf("%s\n", res_hex);
                // printf("%s\n", shift_hex);
                // printf("%s + %s (<< %lu) = %s\n", tmp_hex, shift_hex, bit, res_hex);
                free(res_hex);
                free(shift_hex);
                free(tmp_hex);

                bigint_destructor(&bigint_shift);
                bigint_destructor(&tmp);
            }

    return SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 CONVERSION                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
