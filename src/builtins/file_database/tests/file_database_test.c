#include "../file_database.h"

int main()
{
    Blockchain bc = database_create_blockchain("src/builtins/file_database/tests/test.txt");
    blockchain_free_blockchain(bc);
    //printf("%s\n", bc->block->hash);
    /*print_blockchain(bc);
    size_t error_check_blockchain = blockchain_check_blockchain(bc);
    if (error_check_blockchain == 1)
        printf("Blockchain is correct !\n");
    else
        printf("Blockcahin is incorrect (error in block index: %li) !\n", error_check_blockchain);
    blockchain_free_blockchain(bc);*/
    return 0;
}