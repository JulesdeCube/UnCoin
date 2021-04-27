#include "../file_database.h"

int main()
{
    Blockchain bc = database_create_blockchain("src/builtins/file_database/tests/test.txt");
    //printf("%s\n", bc->block->hash);
    return 0;
}
