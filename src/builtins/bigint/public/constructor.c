#include "utils/bool.h"
#include "utils/error.h"
#include "destructor.h"

int BigInt_constructor(BigInt *new_bigInt)
{
    BigInt bigInt = malloc(sizeof(struct s_BigInt));
    if(bigInt == NULL)
        return NO_SPACE;
    bigInt->exhibitor = 0;
    bigInt->is_positive = POSITIVE;
    /*int error = buffer_constructor_size(&bigInt->buffer,0);
    if(error != SUCCESS)
    {
        bigInt_destroy(&bigInt);
        return error;
    }
    */
    *new_bigInt = bigInt;
    return SUCCESS;
}

BigInt from_int(int i)
{
    struct BigInt bigi = malloc(sizeof(struct BigInt));

}




BigInt add(BigInt bigint1, BigInt bigint2)
{

}