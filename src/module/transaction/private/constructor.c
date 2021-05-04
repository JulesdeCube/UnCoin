# include "../transaction.h"

int _buffer_fusion(Buffer *result, Buffer buf_list[], size_t n)
{
    if (n == 0)
        return NO_SELF;

    size_t sum_size = 0;
    for (size_t i = 0; i < n; ++i)
        sum_size += buffer_get_size(buf_list[i]);

    Buffer res;
    int error = buffer_constructor_size(&res, sum_size);
    if (error != SUCCESS)
        return error;
    for (size_t i = 0; i < n; ++i)
    {
        memcpy(buffer_get_data(res),
                buffer_get_data(buf_list[i]),
                buffer_get_size(buf_list[i]));
    }

    *result = res;
    return SUCCESS;
}
/*
Buffer _transaction_message(Transaction transaction)
{
    return PoW_hash(transaction->from + transaction->to + transaction->amount);
}*/