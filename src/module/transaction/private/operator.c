# include "../transaction.h"

void _copy_buffer(u_char **data_from_buf_result, Buffer buf_to_copy)
{
    size_t size = buffer_get_size(buf_to_copy);

    // add size
    // convert to size_t pointer
    size_t *d = (size_t *) (*data_from_buf_result);
    // set the value
    *d = size;
    // move the data pointer
    *data_from_buf_result += sizeof(size_t);

    // add the buffer data
    // move the buffer data
    memcpy(*data_from_buf_result, buffer_get_data(buf_to_copy), size);
    // move pointer
    *data_from_buf_result += size;
}

int _create_buf(u_char **data, Buffer *buf_result)
{
    // get size
    // convert to size_t pointer
    size_t *s = (size_t *) (*data);
    // get the value
    size_t size_from = *s;
    // move the data pointer
    *data += sizeof(size_t);

    // Construct new buffer
    int error = buffer_constructor_size(buf_result, size_from);
    if (error != SUCCESS)
        return error;

    // get the buffer data
    // move the buffer data
    memcpy(buffer_get_data(*buf_result), *data, size_from);
    *data += size_from;
    return SUCCESS;
}



/*
Buffer _transaction_message(Transaction transaction)
{
    return PoW_hash(transaction->from + transaction->to + transaction->amount);
}*/