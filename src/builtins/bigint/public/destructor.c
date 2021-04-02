#include "../bigint.h"

void bigint_destructor(BigInt *bigint)
{
    if (bigint == NULL || *bigint == NULL)
        return;

    buffer_destructor_safe(&(*bigint)->buffer);

    free(*bigint);

    *bigint = NULL;
}