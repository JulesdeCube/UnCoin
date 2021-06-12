#ifndef UNCOIN__BUILTINS_FILE_DATABASE__FILE_DATABASE_H_
#define UNCOIN__BUILTINS_FILE_DATABASE__FILE_DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/kernel.h>
#include "../buffer/buffer.h"

#include "../blockchain/blockchain.h"

#define LINE_SIZE 256
#define BUFFER_SIZE 256

#define DIR_PATH "src/builtins/file_database/tests/"

Blockchain database_init();
void database_findFile(Blockchain bc);
void database_readFile_addBlock(Blockchain blockchain, char *path_file);
size_t size_sizet(size_t index);
char *size_t_to_char(size_t index,size_t len);
void database_createFile_FromBlock(Blockchain bc);
char *database_findLast_block(size_t last_block_index);

#endif // UNCOIN__BUILTINS_FILE_DATABASE__FILE_DATABASE_H_
