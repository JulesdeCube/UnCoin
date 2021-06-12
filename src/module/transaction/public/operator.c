#include "../transaction.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PRIVATE DEFINTIONS                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void _copy_buffer(u_char **data_from_buf_result, Buffer buf_to_copy);

int _create_buf(u_char **data, Buffer *buf_result);
//Buffer _transaction_message(Transaction transaction);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PUBLIC IMPLEMENTATIONS                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/*
int transaction_is_valid(Transaction transaction, bool *result)
{

    if (transaction == NULL)
        return NO_SELF;
    result = transaction->from == NULL || Pki_isValid(transaction_message(transaction),
                                                    transaction->signature,
                                                    transaction->from));
    (void) result;
    return SUCCESS;
}*/

int transaction_to_buffer(Transaction t, Buffer *buf_result)
{
    if (t == NULL)
        return NO_SELF;

    // Sum the size of all element to have the final size
    size_t size = buffer_get_size(t->to)
                 + buffer_get_size(t->from)
                 + sizeof(size_t) * 2
                 + sizeof(time_t)
                 + sizeof(long long unsigned int);
    int error = buffer_constructor_size(buf_result, size);
    if (error != SUCCESS)
        return error;

    u_char *data = buffer_get_data(*buf_result);
    // Copy the buffers "from" and "to"
    _copy_buffer(&data, t->from);
    _copy_buffer(&data, t->to);

    // Add amount
    // convert to good type pointer
    void *wait = data;
    long long unsigned int *d = wait;
    // set the value
    *d = t->amount;
    // move data pointer
    data += sizeof(long long unsigned int);

    // Add time
    // convert to time_t pointer
    wait = data;
    time_t *time = wait;
    // set the value
    *time = t->date;

    return SUCCESS;
}

int transaction_from_buffer(Transaction *t, Buffer *buf)
{
    if (buf == NULL)
        return NO_SELF;
    size_t size = buffer_get_size(*buf);
    if (size == 0)
        return NO_SELF;
    u_char *data = buffer_get_data(*buf);

    // Get "from" and "to"
    Buffer buf_from, buf_to;
    int error = _create_buf(&data, &buf_from);
    if (error != SUCCESS)
        return error;
    error = _create_buf(&data, &buf_to);
    if (error != SUCCESS)
        return error;
    // Get amount
    // convert to good type pointer
    void *wait = data;
    long long unsigned int *d = wait;
    // set the value
    long long unsigned int amount = *d;

    // move data pointer
    data += sizeof(long long unsigned int);

    // Get time
    // convert to time_t pointer
    wait = data;
    // set the value
    time_t *time = wait;

    // Call transaction construct
    return transaction_constructor_client(t, buf_from, buf_to, amount, *time);
}

int buf_trans_encrypt(Buffer *buf_to_encrypt, Buffer private_key, Buffer *result)
{
    // TODO :> Make a real encrypt c:
    (void) private_key;
    return buffer_constructor_buffer(result, *buf_to_encrypt);
}

int buf_trans_decrypt(Buffer *buf_to_decrypt, Buffer public_key, Buffer *result)
{
    // TODO :> Make a real decrypt c:
    (void) public_key;
    return buffer_constructor_buffer(result, *buf_to_decrypt);
}