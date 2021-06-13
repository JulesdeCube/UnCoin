#include "../file_database.h"

void print_blockchain(Blockchain bc)
{
    Block b = bc->block;
    while (b != NULL)
    {

        char *str1 = blockchain_block_buffer_to_string(b->previousHash);
        char *str2 = blockchain_block_buffer_to_string(b->hash);

        Buffer from = b->transaction->from;
        Buffer to = b->transaction->to;
        long long unsigned int amout = b->transaction->amount;
        time_t date = b->transaction->date;

        char *str_from = (char*)buffer_get_data(from);
        char *str_to = (char*)buffer_get_data(to);

        /*
        Buffer buff_transa;
        transaction_to_buffer(b->transaction, &buff_transa);
        char *str_transa = (char *) buffer_get_data(buff_transa);
        for(size_t i = 0; i < buffer_get_size(buff_transa); i++)
            printf("%c", str_transa[i]);*/

        printf("\n");
        printf("\n    [%p]\n\n", b);
        printf("    index         : %li,\n", b->index);
        printf("    nonce         : %li,\n", b->nonce);
        printf("    data          : From:%s, To: %s, Amount:%lli, Date:%s\n", str_from,str_to,amout,ctime(&date));
        printf("    previousHash  : %s,\n", str1);
        printf("    hash          : %s,\n", str2);
        printf("    previousBlock : %p\n\n", b->previousBlock);

        b = b->previousBlock;

        //buffer_destructor_safe(&from);
        //buffer_destructor_safe(&to);
    }
}

void print_blockchain_check(Blockchain bc)
{
    size_t error_check_blockchain = blockchain_check(bc);
    if (error_check_blockchain == 1)
        printf("Blockchain is correct !\n");
    else
        printf("Blockcahin is incorrect (error in block index: %li) !\n", error_check_blockchain);
}

int main()
{
    Blockchain bc = database_init();

    //printf("%lo\n",bc->block->nonce);
    /*
    Buffer buff;

    buffer_constructor_str(&buff,"bapti",true);

    */
    //Debug
    print_blockchain(bc);
    print_blockchain_check(bc);

    //database_createFile_FromBlock(bc);

    // Free blockchain
    blockchain_destructor(bc);

    return 0;
}
