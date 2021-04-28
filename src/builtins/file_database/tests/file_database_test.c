#include "../file_database.h"

void print_blockchain(Blockchain bc)
{
    Block b = bc->block;

    while (b != NULL)
    {
        // Print block info
        printf("\n    [%p]\n\n", b);
        printf("    index         : %li,\n", b->index);
        printf("    nonce         : %li,\n", b->nonce);
        printf("    data          : %s,\n", (char *)b->data);
        printf("    previousHash  : %s,\n", b->previousHash);
        printf("    hash          : %s,\n", b->hash);
        printf("    previousBlock : %p\n\n", b->previousBlock);

        b = b->previousBlock;
    }
}

void print_blockchain_check(Blockchain bc)
{
    size_t error_check_blockchain = blockchain_check_blockchain(bc);
    if (error_check_blockchain == 1)
        printf("Blockchain is correct !\n");
    else
        printf("Blockcahin is incorrect (error in block index: %li) !\n", error_check_blockchain);
}

int main()
{
    Blockchain bc = database_init();

    // Debug
    print_blockchain(bc);
    print_blockchain_check(bc);

    // Free blockchain
    blockchain_free_blockchain(bc);

    return 0;
}