#include "../blockchain.h"

void print_blockchain(Blockchain bc)
{
    Block b = bc->block;
    //printf("{\n");
    while (b != NULL)
    {
        printf("\n    [%p]\n\n", b);
        printf("    index         : %li,\n", b->index);
        printf("    nonce         : %li,\n", b->nonce);
        printf("    data          : %s,\n", (char *)b->data);
        printf("    previousHash  : %s,\n", b->previousHash);
        printf("    hash          : %s,\n", b->hash);
        printf("    previousBlock : %p\n", b->previousBlock);
        printf("\n");
        b = b->previousBlock;
    }
}

int main()
{
    Blockchain bc = blockchain_create_blockchain();

    blockchain_add_block(bc, 1, "Hello 1");
    //printf("hash 1 0000111: %s\n", bc->block->hash);

    blockchain_add_block(bc, 2, "Hello 2");
    //printf("hash 2 0000222: %s\n", bc->block->hash);

    blockchain_add_block(bc, 16, "Hello 16");
    //printf("hash 16 0000default: %s\n", bc->block->hash);

    blockchain_add_block(bc, 999, "Hello 999");
    //printf("hash 999 error: %s\n", bc->block->hash);

    print_blockchain(bc);

    size_t error_check_blockchain = blockchain_check_blockchain(bc);
    if (error_check_blockchain == 1)
        printf("Blockchain is correct !\n");
    else
        printf("Blockcahin is incorrect (error in block index: %li) !\n", error_check_blockchain);
    blockchain_free_blockchain(bc);

    return 0;
}