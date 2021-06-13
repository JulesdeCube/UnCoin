#include "../file_database.h"

Blockchain database_init()
{
    // Init blockchain
    Blockchain bc = blockchain_contructor();

    // Find file and add block in blockchain
    database_findFile(bc);

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
            //char *filename = malloc(sizeof(char) * BUFFER_SIZE);
            //if (filename == NULL)
                //errx(EXIT_FAILURE, "Error : malloc filename");

            // Path of file csv in filename
            //filename = strcat(filename, DIR_PATH);
            //filename = strcat(filename, dirent->d_name);
            char *filename;
            asprintf(&filename,"%s%s",DIR_PATH,dirent->d_name);
            // TODO: Verifier si le nom du fichier correspond au hash du block
            /*char *name = strtok(dirent->d_name, ".");
                if (strcmp(bc->block->hash, name))
                    errx(EXIT_FAILURE, "The block %li / %s is not correct in %s.csv",
                         bc->block->index, bc->block->hash, name);*/

            // Transfert info file in block and add in blockchain
            database_readFile_addBlock(bc, filename);

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

void database_readFile_addBlock(Blockchain blockchain, char *path_file)
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
    long long unsigned int amount;
    time_t date;
    int i = 1;

    // TODO: A changer
    while (fgets(line, LINE_SIZE, file) != NULL)
    {
        // Data (char *)
        buff = strtok(line, ";");

        if(i == 1)
        {
            buffer_constructor_str(&from, buff, true);
        }
        if(i == 2)
        {
            buffer_constructor_str(&to, buff, true);
        }
        if(i == 3)
        {
            amount = strtoull(buff, NULL, 10);
        }
        if(i == 4)
        {
            int year = 0, month = 0, day = 0, hour = 0, min = 0,sec = 0;
            if (sscanf(buff, "%4d.%2d.%2d %2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) == 6)
            {
                struct tm breakdown = {0};
                breakdown.tm_year = year - 1900; /* years since 1900 */
                breakdown.tm_mon = month - 1;
                breakdown.tm_mday = day;
                breakdown.tm_hour = hour;
                breakdown.tm_min = min;
                breakdown.tm_sec = sec;
                if ((date = mktime(&breakdown)) == (time_t)-1)
                {
                    printf("Could not convert time input to time_t\n");
                }
                puts(ctime(&date));
            }
        }
        i++;
        //data = database_duplicate_string(buff);
        //printf("%s\n", data);
    }
    /*
    char *str_from = (char*)buffer_get_data(from);
    char *str_to = (char*)buffer_get_data(to);

    printf("%s\n",str_from);
    printf("%s\n",str_to);
    printf("%lli\n",amount);
    printf("%s\n",ctime(&date));*/

    Transaction transac;
    transaction_constructor_client(&transac,from,to,amount,date);
    //transaction bien pleine

    // Create and add block in blockchain
    blockchain_block_add(blockchain, transac);
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
    char *hash = NULL;//database_duplicate_string((char *)buffer_get_data(bc->block->hash));;

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

    if (fic != NULL)
    {
        //char *from = malloc(sizeof(char)*100);
        char *from = (char *)buffer_get_data(bc->block->transaction->to);
        //strcat(from,froms);

        strcat(from,";\n");
        //char *to = database_duplicate_string((char *)buffer_get_data(bc->block->transaction->to));
        //strcat(to,";\n");
        //printf("oui\n");
        printf("%s\n",from);
        //printf("%s\n",to);
        //long long unsigned int amount = bc->block->transaction->amount;
        //time_t date = bc->block->transaction->date;
        /*
        if (fputs(from, fic) == EOF)
        {
            printf("oui\n");
            free(from);
            free(filename);
            fclose(fic);
            errx(EXIT_FAILURE, "Error file_database_createFile write in file");
        }
        printf("from append\n");

        if (fputs(to, fic) == EOF)
        {
            free(filename);
            fclose(fic);
            errx(EXIT_FAILURE, "Error file_database_createFile write in file");
        }
        printf("to append\n");*/
        //int error = fprintf(fic, "%llu",amount);
        //printf("%i\n",error);
        /*
        if ()
        {
            printf("oui\n");
            free(filename);
            fclose(fic);
            errx(EXIT_FAILURE, "Error file_database_createFile write in file");
        }
        printf("oui\n");
        if (fprintf(fic, "%s;\n",ctime(&date)) != 1)
        {
            free(filename);
            fclose(fic);
            errx(EXIT_FAILURE, "Error file_database_createFile write in file");
        }*/
        free(from);
        //free(to);
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
