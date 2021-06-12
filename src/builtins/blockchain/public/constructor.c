#include "../blockchain.h"

///////////////////////////////////////////
//                                       //
//         TOOLS                         //
//                                       //
///////////////////////////////////////////

size_t blockchain_size(long x)
{
    size_t n = 1;
    while (x != 0)
    {
        n++;
        x /= 10;
    }
    return n;
}

void blockchain_block_hash(Block block)
{
    // PreviousHash
    char *previousHash = NULL;
    if (block->previousHash != NULL)
        previousHash = blockchain_block_buffer_to_string(block->previousHash);

    // Length for malloc
    size_t len_index = blockchain_size(block->index);
    size_t len_nonce = blockchain_size(block->nonce);
    size_t len = len_index + len_nonce + strlen((char *)block->data);
    if (block->previousHash != NULL)
        len += strlen(previousHash);

    // Allocate Final data : index + data + previousHash + nonce
    char *data = malloc(sizeof(char) * len);

    // Malloc size_t/long -> char *
    char *index = malloc(sizeof(char) * len_index);
    char *nonce = malloc(sizeof(char) * len_nonce);
    sprintf(index, "%ld", block->index);
    sprintf(nonce, "%ld", block->nonce);

    // Append in data
    data = strcat(data, index);
    data = strcat(data, nonce);
    data = strcat(data, (char *)block->data);
    if (block->previousHash != NULL)
        data = strcat(data, previousHash);

    //printf("%s\n", data);

    // Hash in buff
    create_buff_hashed(&block->hash, data,len);

    // Free
    free(data);
    free(index);
    free(nonce);
    free(previousHash);
}

char *blockchain_block_buffer_to_string(Buffer buff)
{
    char *str;
    buffer_to_hex(buff, &str, NULL);

    return str;
}

///////////////////////////////////////////
//                                       //
//             BLOCK                     //
//                                       //
///////////////////////////////////////////

Block blockchain_block_constructor(void *data, Block lastBlock)
{
    // Allocate for block
    Block block = calloc(6, sizeof(Block));
    if (block == NULL)
        errx(EXIT_FAILURE, "Error : malloc new block");

    // Previous block
    block->previousBlock = lastBlock;

    // Previous hash
    if (lastBlock != NULL)
    {
        block->index = lastBlock->index + 1;
        block->previousHash = lastBlock->hash;
    }
    else
        block->index = 0;

    // Data
    block->data = data;

    // Mining (hash and nonce)
    blockchain_block_mine(block);

    return block;
}

int blockchain_block_check(Block block)
{
    return blockchain_block_proof_of_work(block);
}

///////////////////////////////////////////
//                                       //
//             BLOCKCHAIN                //
//                                       //
///////////////////////////////////////////

Blockchain blockchain_contructor()
{
    // Create genesis block (first block)
    Block genesisBlock = blockchain_block_constructor(GENESIS_DATA, NULL);

    // Allocate for blockchain
    Blockchain blockchain = malloc(sizeof(Blockchain));
    if (blockchain == NULL)
        errx(EXIT_FAILURE, "Error : malloc new blockchain");

    // Init blockchain
    blockchain->block = genesisBlock;

    return blockchain;
}

void blockchain_block_add(Blockchain blockchain, void *data)
{
    // Create new block with data and add in blockchain
    Block newBlock = blockchain_block_constructor(data, blockchain->block);

    // Add new block in blockchain
    blockchain->block = newBlock;
}

int blockchain_check(Blockchain blockchain)
{
    // Get current and previous block in blockchain;
    Block current = blockchain->block;
    Block previous = current->previousBlock;

    // While previous is not a genesis block
    while (previous != NULL)
    {
        // Check block
        if (blockchain_block_check(current) != 1)
            return current->index;

        // Init string and error
        char *previousHash_Str = blockchain_block_buffer_to_string(previous->hash);
        char *currentPreviousHash_Str = blockchain_block_buffer_to_string(current->previousHash);
        int error_hash = strcmp(previousHash_Str, currentPreviousHash_Str);

        // Free string
        free(previousHash_Str);
        free(currentPreviousHash_Str);

        // Error hash previous
        if (error_hash > 0)
            return current->index;

        current = previous;
        previous = current->previousBlock;
    }

    return 1;
}

///////////////////////////////////////////
//                                       //
//             PROOF OF WORK             //
//                                       //
///////////////////////////////////////////

void blockchain_block_mine(Block block)
{
    // First hash
    blockchain_block_hash(block);

    // Hash to "000..."
    while (blockchain_block_check(block) != 1)
    {
        block->nonce++;
        blockchain_block_hash(block);
    }
}

int blockchain_block_proof_of_work(Block block)
{
    char *hash = blockchain_block_buffer_to_string(block->hash);
    //printf("hash : %s\n", hash);

    for (size_t i = 0; i < PROOF; i++)
    {
        if (hash[i] != '0')
            return 0;
    }
    return 1;
}

///////////////////////////////////////////
//                                       //
//         FREE / DESTROY                //
//                                       //
///////////////////////////////////////////

void block_destructor(Block block)
{
    //if (block->hash != NULL)
    //    buffer_destructor(&block->hash);
    //if (block->previousHash != NULL)
    //    buffer_destructor(&block->previousHash);
    free(block);
}

void blockchain_destructor(Blockchain blockchain)
{
    while (blockchain->block != NULL)
    {
        Block current_block = blockchain->block;
        blockchain->block = current_block->previousBlock;
        block_destructor(current_block);
    }
    free(blockchain);
}