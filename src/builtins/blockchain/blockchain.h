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
#include "../../module/hash_table/hash_table.h"

#define PROOF 5 // 5 premier zero dans le hash
#define GENESIS_DATA "Genesis block"

struct block
{
    Buffer previousHash;

    size_t index;
    long nonce;

    //Transaction transaction;
    //TODO : add hashtable
    Transaction data[];

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

Block blockchain_block_constructor(void *data, Block lastBlock);
int blockchain_block_check(Block block);

Blockchain blockchain_contructor();
void blockchain_block_add(Blockchain blockchain, void *data);
int blockchain_check(Blockchain blockchain);

void blockchain_block_mine(Block block);
int blockchain_block_proof_of_work(Block block);

void block_destructor(Block block);
void blockchain_destructor(Blockchain blockchain);

/*
struct transaction
{
    char *from; // Key public of me
    char *to;   // Key public of him
    long amount;
    char *signature;
};

typedef struct transaction *Transaction;

Transaction transaction_contructor(char *from, char *to, long amount, char *privKey);
Buffer transaction_message(Transaction transaction);
int isValid(Transaction transaction);*/

#endif //UNCOIN__BUILTINS_BLOCKCHAIN__BLOCKCHAIN_H_