#include "../transaction.h"

size_t get_transaction_size(Transaction t)
{
    if (t == NULL)
        return 0;
    size_t size = 0;
    size += buffer_get_size(t->from);
    size += buffer_get_size(t->to);
    size += sizeof(long long unsigned int);
    size += sizeof(time_t);
    return size;
}