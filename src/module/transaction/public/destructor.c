# include "../transaction.h"

void transaction_destructor(Transaction transac)
{
    if(transac == NULL)
        return;
    buffer_destructor_safe(&(transac->from));
    buffer_destructor_safe(&(transac->to));
    buffer_destructor_safe(&(transac->message));

    free(transac);
} 