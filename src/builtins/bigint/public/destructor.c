#include "../bigint.h"

void bigint_destructor(BigInt *bigint)
{
    free(bigint);
}