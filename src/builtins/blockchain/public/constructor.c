#include "../blockchain.h"

#define PROOF 4 // 4 premier zero dans le hash
#define GENESIS_DATA "Genesis block"

struct block
{
    size_t index;
    size_t nonce; // Random num by miners (proof)

//    size_t timestamp;
    void *data;

    char* previousHash;
    char* hash;

    struct block *previousBlock;
};

struct blockchain
{
    struct block *block;
};

typedef struct blockchain *Blockchain;
typedef struct block *Block;


///////////////////////////////////////////
//                                       //
//         HASH                          //
//                                       //
///////////////////////////////////////////

size_t blockchain_hash(Block block)
{
    // TODO pour Vincent
}


///////////////////////////////////////////
//                                       //
//         CREATE                        //
//                                       //
///////////////////////////////////////////

Block blockchain_create_new_block(size_t nonce, void* data, Block last_block)
{
    // Allocate for block
    Block block = calloc(6, sizeof(Block));
    if (block == NULL)
        errx(EXIT_FAILURE, "Error : malloc new block");

    // Init value of block
    if (last_block != NULL)
    {
        block->index = last_block->index + 1;
        block->previousHash = last_block->hash;
    }

    block->nonce = nonce;
    block->data = data;

    // Create hash for this new block
    block->hash = blockchain_hash(block);

    // Init link
    block->previousBlock = last_block;

    return block;
}

Blockchain blockchain_create_blockchain()
{
    // Create genesis block (first block)
    Block genesis_block = blockchain_create_new_block(0, GENESIS_DATA, NULL);

    // Allocate for blockchain
    Blockchain blockchain = malloc(sizeof(Blockchain));
    if (blockchain == NULL)
        errx(EXIT_FAILURE, "Error : malloc new blockchain");

    // Init blockchain
    blockchain->block = genesis_block;

    return blockchain;
}


///////////////////////////////////////////
//                                       //
//         FREE / DESTROY                //
//                                       //
///////////////////////////////////////////

void blockchain_free_block(Block block)
{
    free(block);
}

void blockchain_free(Blockchain blockchain)
{
    while (blockchain->block != NULL)
    {
        Block current_block = blockchain->block;
        blockchain->block = current_block->previousBlock;
        blockchain_free_block(current_block);
    }
    free(blockchain);
}


///////////////////////////////////////////
//                                       //
//         CHECK / VERIFICATION          //
//                                       //
///////////////////////////////////////////

int blockchain_proof_of_woork_block(Block block)
{
    char* hash = block->hash;
    for (size_t i = 0; i < PROOF; i++)
    {
        if (hash[i] != 0)
            return 0;
    }
    return 1;
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

        if (blockchain_proof_of_woork_block(Block block) == 0)
            return 0;

        current = previous;
        previous = current->previousBlock;
    }

    return 1;
}


///////////////////////////////////////////
//                                       //
//         ADD                           //
//                                       //
///////////////////////////////////////////

int blockchain_add_block(Blockchain blockchain, size_t nonce, void *data)
{
    // Blockchain doesn't exist
    if (blockchain == NULL)
        blockchain = blockchain_create_blockchain();

    // Get last block in blockchain
    Block last_block = blockchain->block;

    // Create new block with data
    Block new_block = blockchain_create_new_block(nonce, data, last_block->hash);

    // Block can't be add because of proof of work
    if (blockchain_proof_of_woork_block(Block block) == 0)
    {
        blockchain_free_block(Block);
        return 0;
    }

    // Add new block in blockchain
    blockchain->block = new_block;

    // Block added
    return 1;
}
