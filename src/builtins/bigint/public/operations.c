#include "../bigint.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Arithmetique                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Bitwise                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             CONVERSION                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int bigint_operations_to_int(BigInt *bigint)
{
    Buffer int_buffer = bigint->bufeer;
    size_t size = buffer_get_size(int_buffer);
    u_char *data = buffer_get_data(int_buffer) + size - 1;
    int res = 0;

    if(size == 0)
        return res;
    else if (size > sizeof(int))
        res = *(d-sizeof(int));
    else
        for (size_t i = 0; size > 0 && i < sizeof(int); size--, data--)
            res |= *d << (i << 3);

    if(bigint->sign == NEGATIVE)
        res = -res;

    return res;
}

bool bigint_operations_to_bool(BigInt *bigint)
{
    if(bigint->exhibitor == NEGATIVE)
        return false;
    return true;
}

long long int bigint_operations_to_long_long_int(BigInt *bigint)
{
    Buffer int_buffer = bigint->bufeer;
    size_t size = buffer_get_size(int_buffer);
    u_char *data = buffer_get_data(int_buffer) + size - 1;
    int res = 0;

    if(size == 0)
        return res;
    else if (size > sizeof(long long int))
        res = *(d-sizeof(long long int));
    else
        for (size_t i = 0; size > 0 && i < sizeof(long long int); size--, data--)
            res |= *d << (i << 3);

    if(bigint->sign == NEGATIVE)
        res = -res;

    return res;
}

Buffer bigint_operations_to_buffer(BigInt *bigint)
{
    Buffer new_buffer;
    buffer_constructor_buffer(&new_buffer, bigint->buffer);
    return new_buffer;
}

char *bigint_operations_to_string(BigInt *bigint)
{

}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Comparaison                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////