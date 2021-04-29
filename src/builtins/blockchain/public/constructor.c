#include "../blockchain.h"

///////////////////////////////////////////
//                                       //
//         HASH                          //
//                                       //
///////////////////////////////////////////

void blockchain_init_bufferHash(Block block, int increment_nonce)
{
    if (increment_nonce == 1)
        block->nonce++;

    char *nonce_str = malloc(sizeof(char) * 256);

    sprintf(nonce_str, "%ld", block->nonce);

    nonce_str = strcat(nonce_str, (char *)block->data);

    create_buff_hashed(&block->hash, nonce_str);

    free(nonce_str);
}

char *blockchain_hash_str(Buffer buff_hash)
{
    char *hash;
    buffer_to_hex(buff_hash, &hash, NULL);

    return hash;
}

///////////////////////////////////////////
//                                       //
//         CREATE                        //
//                                       //
///////////////////////////////////////////

Block blockchain_create_new_block(size_t nonce, void *data, Block last_block)
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
    blockchain_init_bufferHash(block, 0);

    // Init link
    block->previousBlock = last_block;

    return block;
}

Blockchain blockchain_create_blockchain()
{
    // Create genesis block (first block)
    Block genesis_block = blockchain_create_new_block(0, GENESIS_DATA, NULL);
    blockchain_block_mining(genesis_block);

    // Only ONE blockchain
    static Blockchain blockchain = NULL;
    if (blockchain != NULL)
        errx(EXIT_FAILURE, "Error : blockchain already exist");

    // Allocate for blockchain
    blockchain = malloc(sizeof(Blockchain));
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

void blockchain_free_blockchain(Blockchain blockchain)
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
    char *hash = blockchain_hash_str(block->hash);
    //printf("hash : %s\n", hash);

    for (size_t i = 0; i < PROOF; i++)
    {
        if (hash[i] != '0')
            return 0;
    }
    return 1;
}

void blockchain_block_mining(Block block)
{
    while (blockchain_proof_of_woork_block(block) != 1)
        blockchain_init_bufferHash(block, 1);
}

int blockchain_check_blockchain(Blockchain blockchain)
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
        /*char *hash_current = blockchain_hash_str(current->hash);
        current->nonce = 0;
        blockchain_init_bufferHash(current, 0);
        blockchain_block_mining(current);

        if (hash_current != blockchain_hash_str(current->hash))
            return current->index;*/

        if (strcmp(blockchain_hash_str(previous->hash), blockchain_hash_str(current->previousHash)) > 1)
            return current->index;

        if (blockchain_proof_of_woork_block(current) == 0)
            return current->index;

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
    Block new_block = blockchain_create_new_block(nonce, data, last_block);

    blockchain_block_mining(new_block);

    /*
    // Block can't be add because of proof of work
    if (blockchain_proof_of_woork_block(new_block) == 0)
    {
        //printf("Block is not correct (nonce incorrect) !\n");
        blockchain_free_block(new_block);
        return 0;
    }
*/
    // Add new block in blockchain
    blockchain->block = new_block;

    // Block added
    return 1;
}
