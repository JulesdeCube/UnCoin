#include "../blockchain.h"

#define GENESIS_TIME "01/01/2021"
#define GENESIS_DATA "Genesis block"

struct block
{
    size_t index;
    size_t timestamp;
    void *data;
    size_t previousHash;
    size_t hash;
    struct block *previousBlock;
};

struct blockchain
{
    struct block *block;
};

typedef struct blockchain *Blockchain;
typedef struct block *Block;

size_t blockchain_hash(Block block)
{
    // TODO
}

Block blockchain_create_new_block(size_t index, char* time, void* data,
                                  size_t previousHash, Block last_block)
{
    // Allocate for block
    Block block = malloc(sizeof(Block));
    if (block == NULL)
        errx(EXIT_FAILURE, "Error : malloc new block");

    // Init value of block
    block->index = index;
    block->timestamp = time;
    block->data = data;
    block->previousHash = previousHash;

    // Create hash for this new block
    block->hash = blockchain_hash(block);

    // Init link
    block->previousBlock = last_block;

    return block;
}

Blockchain blockchain_create_blockchain()
{
    // Create genesis block (first block)
    Block genesis_block = blockchain_create_new_block(0,
                                                      GENESIS_TIME,
                                                      GENESIS_DATA,
                                                      0,
                                                      NULL);

    // Allocate for blockchain
    Blockchain blockchain = malloc(sizeof(Blockchain));
    if (blockchain == NULL)
        errx(EXIT_FAILURE, "Error : malloc new blockchain");

    // Init blockchain
    blockchain->block = genesis_block;

    return blockchain;
}

void blockchain_add_block(Blockchain blockchain, void *data)
{
    // Blockchain doesn't exist
    if (blockchain == NULL)
        blockchain = blockchain_create_blockchain();

    // Get last block in blockchain
    Block last_block = blockchain->block;

    // Create new block with data
    Block new_block = blockchain_create_new_block(last_block->index + 1,
                                                  "01/01/2021",
                                                  data,
                                                  last_block->hash,
                                                  last_block);

    // Add new block in blockchain
    blockchain->block = new_block;
}

int blockchain_check_block(Blockchain blockchain)
{
    // Blockchain doesn't exist
    if (blockchain == NULL)
        return 1;

    // Get current and previous block in blockchain;
    Block current = blockchain->block;
    Block previous = current->previousBlock;

    // While previous is not a genesis block
    while (previous != NULL)
    {
        // Check all hash
        if (blockchain_hash(previous) != current->previousHash)
            return 0;

        if (blockchain_hash(current) != current->hash)
            return 0;

        current = previous;
        previous = current->previousBlock;
    }

    return 1;
}

