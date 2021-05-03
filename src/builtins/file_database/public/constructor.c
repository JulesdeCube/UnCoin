#include "../file_database.h"

Blockchain database_init()
{
    // Init blockchain
    Blockchain bc = blockchain_contructor();

    // Find file and add block in blockchain
    database_findFile(bc);

    return bc;
}

void database_findFile(Blockchain bc)
{
    // Create directory
    DIR *dir = opendir(DIR_PATH);
    if (dir == NULL)
        errx(EXIT_FAILURE, "Error init dir");

    struct dirent *dirent = NULL;

    // TODO: A changer pour choisir le tt dernier block...
    // While file or repository is find
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strcmp(dirent->d_name, ".") != 0 &&
            strcmp(dirent->d_name, "..") != 0 &&
            strchr(dirent->d_name, '.') != NULL &&
            strcmp(strrchr(dirent->d_name, '.'), ".csv") == 0) // C'est un fichier csv
        {
            // Allocate filename
            char *filename = malloc(sizeof(char) * BUFFER_SIZE);
            if (filename == NULL)
                errx(EXIT_FAILURE, "Error : malloc filename");

            // Path of file csv in filename
            filename = strcat(filename, DIR_PATH);
            filename = strcat(filename, dirent->d_name);

            // Transfert info file in block and add in blockchain
            database_readFile_addBlock(bc, filename);

            // Free path of file
            free(filename);

            // TODO: Verifier si le nom du fichier correspond au hash du block
            /*char *name = strtok(dirent->d_name, ".");
                if (strcmp(bc->block->hash, name))
                    errx(EXIT_FAILURE, "The block %li / %s is not correct in %s.csv",
                         bc->block->index, bc->block->hash, name);*/
        }
    }

    closedir(dir);
}

char *database_duplicate_string(const char *str)
{
    // Error
    if (str == NULL)
        return NULL;

    size_t size = strlen(str) + 1;

    // Allocate a new string
    char *dup = malloc(sizeof(char) * size);
    if (dup == NULL)
        errx(EXIT_FAILURE, "Error : malloc dup string");

    // Copy str in dup
    memcpy(dup, str, size);

    return dup;
}

void database_readFile_addBlock(Blockchain blockchain, char *path_file)
{
    // Open file (reading)
    FILE *file = fopen(path_file, "r");
    if (file == NULL)
        errx(EXIT_FAILURE, "Error file_database_init() : %s", path_file);

    // Buffer
    char line[LINE_SIZE];
    char *buff = NULL, *data = NULL;
    //size_t nonce = -1;

    // TODO: A changer
    while (fgets(line, LINE_SIZE, file) != NULL)
    {
        // Data (char *)
        buff = strtok(line, ";");
        data = database_duplicate_string(buff);
	//printf("%s\n", data);

        // Create and add block in blockchain
        blockchain_block_add(blockchain, data);
    }
}
