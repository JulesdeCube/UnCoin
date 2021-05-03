#include "../blockchain.h"

void print_blockchain(Blockchain bc)
{
    Block b = bc->block;
    while (b != NULL)
    {
        char *str1 = blockchain_block_buffer_to_string(b->previousHash);
        char *str2 = blockchain_block_buffer_to_string(b->hash);

        printf("\n    [%p]\n\n", b);
        printf("    index         : %li,\n", b->index);
        printf("    nonce         : %li,\n", b->nonce);
        printf("    data          : %s,\n", (char *)b->data);
        printf("    previousHash  : %s,\n", str1);
        printf("    hash          : %s,\n", str2);
        printf("    previousBlock : %p\n\n", b->previousBlock);

        b = b->previousBlock;
    }
}

int main()
{
    //printf("%s\n", buffer_toString(hash_StringToBuffer("Genesis block805230")));
    Blockchain bc = blockchain_contructor();

    blockchain_block_add(bc, "Hello 1");
    blockchain_block_add(bc, "Hello 2");
    blockchain_block_add(bc, "Hello 3");
    blockchain_block_add(bc, "Hello 4");
    blockchain_block_add(bc, "Hello 1");
    blockchain_block_add(bc, "Hello 2");
    blockchain_block_add(bc, "Hello 3");
    blockchain_block_add(bc, "Hello 4");
    blockchain_block_add(bc, "Hello 1");
    blockchain_block_add(bc, "Hello 2");
    blockchain_block_add(bc, "Hello 3");
    blockchain_block_add(bc, "Hello 4");

    size_t error_check_blockchain = blockchain_check(bc);
    if (error_check_blockchain == 1)
        printf("Blockchain is correct !\n");
    else
        printf("Blockchain is incorrect (error in block index: %li) !\n", error_check_blockchain);

    print_blockchain(bc);
    blockchain_destructor(bc);

    return 0;
}