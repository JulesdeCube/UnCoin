#ifndef UNCOIN__BUILTINS_FILE_DATABASE__FILE_DATABASE_H_
#define UNCOIN__BUILTINS_FILE_DATABASE__FILE_DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../blockchain/blockchain.h"

#define BUFFER_SIZE 256

Blockchain database_create_blockchain(char *path_file);

#endif // UNCOIN__BUILTINS_FILE_DATABASE__FILE_DATABASE_H_
