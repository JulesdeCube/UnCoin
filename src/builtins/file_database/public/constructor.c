#include "../file_database.h"

Blockchain database_init(Buffer privateKey)
{
    // Init blockchain
    Blockchain bc = blockchain_contructor();

    // Find file and add block in blockchain
    database_findFile(bc,privateKey);

    return bc;
}

/*
char *database_findLast_block(size_t last_block_index)
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


            // Free path of file
            free(filename);
            */
// TODO: Verifier si le nom du fichier correspond au hash du block
/*char *name = strtok(dirent->d_name, ".");
                if (strcmp(bc->block->hash, name))
                    errx(EXIT_FAILURE, "The block %li / %s is not correct in %s.csv",
                         bc->block->index, bc->block->hash, name);*/
/*
        }
    }

    closedir(dir);
}*/

void database_findFile(Blockchain bc, Buffer privateKey)
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

            // TODO: Verifier si le nom du fichier correspond au hash du block
            /*char *name = strtok(dirent->d_name, ".");
                if (strcmp(bc->block->hash, name))
                    errx(EXIT_FAILURE, "The block %li / %s is not correct in %s.csv",
                         bc->block->index, bc->block->hash, name);*/

            // Transfert info file in block and add in blockchain
            database_readFile_addBlock(bc, filename,privateKey);

            // Free path of file
            free(filename);
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

void database_readFile_addBlock(Blockchain blockchain, char *path_file, Buffer private_key)
{
    // Open file (reading)
    FILE *file = fopen(path_file, "r");
    if (file == NULL)
        errx(EXIT_FAILURE, "Error file_database_init() : %s", path_file);

    // Buffer
    char line[LINE_SIZE];
    char *buff = NULL;//, *data = NULL;
    //size_t nonce = -1;

    Buffer from;
    Buffer to;
    double amount;
    Buffer signature;
    Buffer date;
    int i = 1;

    // TODO: A changer
    while (fgets(line, LINE_SIZE, file) != NULL)
    {
        // Data (char *)
        buff = strtok(line, ";");

        if(i == 1)
        {
            from = buffer_constructor_str(&from, buff, true);
            i++;
        }
        if(i == 2)
        {
            to = buffer_constructor_str(&to, buff, true);
            i++;
        }
        if(i == 3)
        {
            //amount = ??;
            i++;
        }
        if(i == 4)
        {
            to = buffer_constructor_str(&signature, buff, true);
            i++;
        }
        if(i == 5)
        {
            to = buffer_constructor_str(&date, buff, true);
            i++;
        }
        //data = database_duplicate_string(buff);
        //printf("%s\n", data);
    }
    Transaction *transac;
    transaction_constructor(transac, from, to, amount, private_key);
    
    // Create and add block in blockchain
    blockchain_block_add(blockchain, data);
}

size_t size_sizet(size_t index)
{
    size_t n = index;
    size_t len = 0;
    while (n > 0)
    {
        len++;
        n /= 10;
    }
    return len;
}

void database_createFile_FromBlock(Blockchain bc)
{
    //creation of the filename
    //index of the block in char *
    size_t len_index = size_sizet(bc->block->index);
    char *filename = malloc(sizeof(char) * len_index);
    sprintf(filename, "%zu", bc->block->index);

    //adding _
    strcat(filename, "_");

    //adding the hash
    char *hash = NULL;
    if (buffer_to_hex(bc->block->hash, &hash, NULL) != SUCCESS)
        printf("BUG\n");

    strcat(hash, ".csv");
    strcat(filename, hash);
    //free(filename);

    char *filedir = malloc(sizeof(char)*33);
    sprintf(filedir, "%s", "src/builtins/file_database/tests/");
    strcat(filedir,filename);
    //printf("%s\n",filedir);

    //écriture
    FILE *fic = NULL;
    fic = fopen(filedir, "w");
    /*
    char *transactions = NULL;
    if(buffer_to_hex(bc->block->h, &transactions, NULL) != SUCCESS)
        printf("BUG\n");*/

    if (fic != NULL)
    {
        char transactions[10] = "reussi";
        if (fputs(transactions, fic) == EOF)
        {
            free(filename);
            fclose(fic);
            errx(EXIT_FAILURE, "Error file_database_createFile write in file");
        }
        fclose(fic);
        free(filename);
    }
    else
    {
        fclose(fic);
        free(filename);
        errx(EXIT_FAILURE, "Error file_database_createFile open file");
    }
}
