# include "../transaction.h"

void transaction_destructor(Transaction transac)
{
    if(transac == NULL)
        return;

    buffer_destructor_safe(&(transac->from));
    buffer_destructor_safe(&(transac->to));
    buffer_destructor_safe(&(transac->signature));
    buffer_destructor_safe(&(transac->date));

    free(transac);
}