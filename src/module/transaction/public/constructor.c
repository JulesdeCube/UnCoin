# include "../transaction.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               PRIVATE DEFINITIONS                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PUBLIC IMPLEMENTATIONS                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int transaction_constructor_client(Transaction *p_tran,
                            Buffer from,
                            Buffer to,
                            long long unsigned int amount,
                            time_t time)
{
    // get the struct malloc
    Transaction tran = malloc(sizeof(struct s_transaction));

    // if we can't malloc it return an error
    if (tran == NULL)
        return NO_SPACE;

    // initialize the properties
    tran->from = from;
    tran->to = to;
    tran->amount = amount;
    tran->message = NULL;
    tran->date = time;

    // return the transaction
    *p_tran = tran;

    return SUCCESS;
}