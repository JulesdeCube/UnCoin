#include "../bigint.h"

void bigint_destructor(BigInt *bigint)
{
    buffer_destructor_safe(&bigint->buffer);
    free(bigint);
}