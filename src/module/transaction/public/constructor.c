# include "../transaction.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               PRIVATE DEFINITIONS                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int _buffer_fusion(Buffer *result, Buffer buf_list[], size_t n);

//Buffer _transaction_message(Transaction transaction);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PUBLIC IMPLEMENTATIONS                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int transaction_constructor(Transaction *p_tran,
                            Buffer from,
                            Buffer to,
                            long amount,
                            Buffer privKey)
{
    // get the struct malloc
    Transaction tran = malloc(sizeof(struct s_transaction));

    // if we can't malloc it return an error
    if (tran == NULL)
        return NO_SPACE;

    // initialize the porperties
    tran->from = from;
    tran->to = to;
    tran->amount = amount;

    // TO CHANGE WITH CURRENT DATE
    tran->date = "04/05/2021";

    Buffer buf_to_hash;
    Buffer buf_list[] = {from, to, privKey};
    int error = _buffer_fusion(&buf_to_hash, buf_list, 3);
    if (error != SUCCESS)
        return error;

    Buffer hkey;
    error = hash(&hkey, &buf_to_hash);
    if (error != SUCCESS)
        return error;

    tran->signature = hkey;
    // IN OTHER WAY
    /*
    Pki_encrypt(tran, _transaction_message(tran), privKey);
    */
    // return the transaction
    *p_tran = tran;

    return SUCCESS;
}