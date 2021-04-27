#ifndef UNCOIN__BUILTINS_BLOCKCHAIN__BLOCKCHAIN_H_
#define UNCOIN__BUILTINS_BLOCKCHAIN__BLOCKCHAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../../utils/bool.h"

#define PROOF 4 // 4 premier zero dans le hash
#define GENESIS_DATA "Genesis block"

struct block
{
    size_t index;
    size_t nonce; // Random num by miners (proof)

    //    size_t timestamp;
    void *data;

    char *previousHash;
    char *hash;

    struct block *previousBlock;
};

struct blockchain
{
    struct block *block;
};

typedef struct blockchain *Blockchain;
typedef struct block *Block;

//char *blockchain_hash(Block block)

//Block blockchain_create_new_block(size_t nonce, void *data, Block last_block);
Blockchain blockchain_create_blockchain();

//void blockchain_free_block(Block block);
void blockchain_free_blockchain(Blockchain blockchain);

//int blockchain_proof_of_woork_block(Block block);
int blockchain_check_blockchain(Blockchain blockchain);

int blockchain_add_block(Blockchain blockchain, size_t nonce, void *data);

#endif //UNCOIN__BUILTINS_BLOCKCHAIN__BLOCKCHAIN_H_