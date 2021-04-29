#include "../blockchain.h"

void print_blockchain(Blockchain bc)
{
    Block b = bc->block;
    //printf("{\n");
    while (b != NULL)
    {
        char *str1;
        buffer_to_hex(b->previousHash, &str1, NULL);
        char *str2;
        buffer_to_hex(b->hash, &str2, NULL);

        printf("\n    [%p]\n\n", b);
        printf("    index         : %li,\n", b->index);
        printf("    nonce         : %li,\n", b->nonce);
        printf("    data          : %s,\n", (char *)b->data);
        printf("    previousHash  : %s,\n", str1);
        printf("    hash          : %s,\n", str2);
        printf("    previousBlock : %p\n", b->previousBlock);
        printf("\n");
        b = b->previousBlock;
    }
}

void gen_random(char *s, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        int randomChar = rand() % (26 + 26 + 10);
        if (randomChar < 26)
            s[i] = 'a' + randomChar;
        else if (randomChar < 26 + 26)
            s[i] = 'A' + randomChar - 26;
        else
            s[i] = '0' + randomChar - 26 - 26;
    }
    s[len] = 0;
}

int main()
{
    Blockchain bc = blockchain_create_blockchain();

    size_t len = 10;
    char *s = malloc(sizeof(char) * len);
    gen_random(s, len);
    blockchain_add_block(bc, 1, s);
    //printf("hash 1 0000111: %s\n", bc->block->hash);

    blockchain_add_block(bc, 2, "Hello 2");
    //printf("hash 2 0000222: %s\n", bc->block->hash);

    blockchain_add_block(bc, 16, "Hello 16");
    //printf("hash 16 0000default: %s\n", bc->block->hash);

    blockchain_add_block(bc, 999, "nduiklkebide");
    //printf("hash 999 error: %s\n", bc->block->hash);

    size_t error_check_blockchain = blockchain_check_blockchain(bc);
    if (error_check_blockchain == 1)
        printf("Blockchain is correct !\n");
    else
        printf("Blockchain is incorrect (error in block index: %li) !\n", error_check_blockchain);

    print_blockchain(bc);
    blockchain_free_blockchain(bc);

    return 0;
}