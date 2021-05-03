# include "../transaction.h"

Buffer _transaction_message(Transaction transaction)
{
    return PoW_hash(transaction->from + transaction->to + transaction->amount);
}