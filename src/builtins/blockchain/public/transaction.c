/*#include "../blockchain.h"

Transaction transaction_contructor(char *from, char *to, long amount, char *privKey)
{
    Transaction transaction = calloc(4, sizeof(Transaction));
    if (transaction == NULL)
        errx(EXIT_FAILURE, "Error : malloc new transaction");

    transaction->from = from;
    transaction->to = to;
    transaction->amount = amount;
    Pki_encrypt(transaction, transaction_message(transaction), privKey);
}

Buffer transaction_message(Transaction transaction)
{
    return PoW_hash(transaction->from + transaction->to + transaction->amount);
}

int isValid(Transaction transaction)
{
    return transaction->from == NULL || Pki_isValid(transaction_message(transaction),
                                                    transaction->signature,
                                                    transaction->from);
}*/