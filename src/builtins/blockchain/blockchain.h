#ifndef UNCOIN__BUILTINS_BLOCKCHAIN__BLOCKCHAIN_H_
#define UNCOIN__BUILTINS_BLOCKCHAIN__BLOCKCHAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <strings.h>
#include "../../utils/bool.h"
#include "../buffer/buffer.h"
#include "../../module/hash/hash.h"
#include "../../module/hash/tests/hash_tools.h"
#include "../../module/transaction/transaction.h"

#define PROOF 5 // 5 premier zero dans le hash

struct block
{
    Buffer previousHash;

    size_t index;
    long nonce;

    Transaction transaction;

    Buffer hash;
    struct block *previousBlock;
};

struct blockchain
{
    struct block *block;
};

typedef struct blockchain *Blockchain;
typedef struct block *Block;

size_t blockchain_size(long nonce);
void blockchain_block_hash(Block block);
char *blockchain_block_buffer_to_string(Buffer buff);

Block blockchain_block_constructor(Transaction transaction, Block lastBlock);
int blockchain_block_check(Block block);

Blockchain blockchain_contructor();
void blockchain_block_add(Blockchain blockchain, Transaction transaction);
int blockchain_check(Blockchain blockchain);

void blockchain_block_mine(Block block);
int blockchain_block_proof_of_work(Block block);

void block_destructor(Block block);
void blockchain_destructor(Blockchain blockchain);

#endif //UNCOIN__BUILTINS_BLOCKCHAIN__BLOCKCHAIN_H_