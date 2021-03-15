#include "utils/bool.h"
#include "utils/error.h"
#include "destructor.h"

void BigInt_destructor(BigInt *bigInt)
{
    free(bigInt);
}