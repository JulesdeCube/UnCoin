#include "../file_database.h"

Blockchain database_create_blockchain(char *path_file)
{
    // Open file (reading)
    FILE *file = fopen(path_file, "r");
    if (file == NULL)
        errx(EXIT_FAILURE, "Error file_database_init()");

    Blockchain blockchain = blockchain_create_blockchain();

    // Buffer
    char buff[BUFFER_SIZE];

    while (fgets(buff, BUFFER_SIZE, file) != NULL)
    {
        //printf("%s", buff);
        size_t i = 0;

        while (buff[i] != ',' && buff[i] != '\n')
            i++;

        char *str_nonce = malloc(sizeof(char) * i);
        strncpy(str_nonce, buff, i);

        // Transforme str en int, size_t
        size_t nonce = atoi(str_nonce);

        char *data = malloc(sizeof(char) * (256 - i));
        strncpy(data, buff + i + 1, 256 - i);
        data[strlen(data) - 1] = '\0';

        //printf("Nonce : %li\nData : %s\n", nonce, data);
        blockchain_add_block(blockchain, nonce, data);
        free(str_nonce);
    }

    return blockchain;
}