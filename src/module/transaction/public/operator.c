#include "../transaction.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PRIVATE DEFINTIONS                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PUBLIC IMPLEMENTATIONS                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int transaction_is_valid(Transaction transaction, bool *result)
{
    if (transaction == NULL)
        return NO_SELF;
    result = transaction->from == NULL || Pki_isValid(transaction_message(transaction),
                                                    transaction->signature,
                                                    transaction->from));
    return SUCCESS;
}