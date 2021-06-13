/*#include "../blockchain.h"

///////////////////////////////////////////
//                                       //
//             HASH                      //
//                                       //
///////////////////////////////////////////

char *string_cat(char *str1, char *str2)
{
    char *str = malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
    if (str == NULL)
        errx(EXIT_FAILURE, "Error : malloc string cat");

    str[0] = 0;

    size_t i = 0, j = 0;
    for (; i < strlen(str1); i++)
        str[i] = str1[i];
    for (; j < strlen(str2); j++)
        str[i + j] = str2[j];

    //if (strlen(str) > strlen(str1) + strlen(str2))
    str[strlen(str1) + strlen(str2)] = '\0';

    //str = strcat(str, str1);
    //str = strcat(str, str2);

    return str;
}

size_t nonce_comp(long nonce)
{
    size_t n = 0;

    while (nonce != 0)
    {
        n++;
        nonce /= 10;
    }

    return n;
}

char *string_long(long nonce)
{
    char *str = malloc(sizeof(char) * (nonce_comp(nonce) + 1));
    sprintf(str, "%ld", nonce);
    str[nonce_comp(nonce)] = 0;
    return str;
}

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

Buffer hash_StringToBuffer(char *string_hash)
{
    Buffer buff;
    create_buff_hashed(&buff, string_hash);

    return buff;
}

char *buffer_toString(Buffer buff)
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

//Block block_contructor(Transaction transaction, Block lastBlock)
Block block_constructor(void *data, Block lastBlock)
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
    {
        block->index = 0;
        //block->previousHash = NULL;
    }

    // Data
    //block->transaction = transaction;
    block->data = data;

    // Mining (hash and nonce)
    block_mine(block);

    return block;
}

// Transaction...
//Block block_createGenesis(char *pubKey, char *privKey, long amount)
//{
//    Transaction transaction = transaction_constructor(NULL, pubKey, amount, privKey);
//    Block genesisBlock = block_contructor(transaction, NULL);
//    return genesisBlock;
//}

int blockchain_proof_of_woork_block(Block block)
{
    char *hash = buffer_toString(block->hash);
    //printf("hash : %s\n", hash);

    for (size_t i = 0; i < PROOF; i++)
    {
        if (hash[i] != '0')
            return 0;
    }
    return 1;
}

void block_mine(Block block)
{
    while (blockchain_proof_of_woork_block(block) != 1)
        blockchain_init_bufferHash(block, 1);
}

void block_mine(Block block)
{
    char *data0 = NULL;
    if (block->previousHash != NULL)
    {
        //char *data0 = transaction_message(block->transaction) + block->previousHash;
        char *previousHash_Str = buffer_toString(block->previousHash);
        data0 = string_cat((char *)block->data, previousHash_Str);
        free(previousHash_Str);
    }
    else
        data0 = (char *)block->data;

    block->nonce = PoW_findNonce(data0);

    char *nonce_Str = string_long(block->nonce);
    char *msg = string_cat(data0, nonce_Str);
    block->hash = hash_StringToBuffer(msg);

    printf("%s\n", msg);

    //if (block->previousHash != NULL)
    //    free(data0);

    free(nonce_Str);
    free(msg);
}

int block_isValid(Block block)
{
    return blockchain_proof_of_woork_block(block);
    char *previousHashStr = buffer_toString(block->previousHash);
    //char *message = string_cat(block->transaction, previousHashStr);
    char *message = string_cat((char *)block->data, previousHashStr);

    int error_validNonce = PoW_isValidNonce(message, block->nonce);

    free(previousHashStr);
    free(message);

    return error_validNonce;
}

///////////////////////////////////////////
//                                       //
//             BLOCKCHAIN                //
//                                       //
///////////////////////////////////////////

//Blockchain blockchain_contructor(char *pubKey, char *privKey, long amount)
Blockchain blockchain_contructor()
{
    // Create genesis block (first block)
    //Block genesisBlock = block_createGenesis(pubKey, privKey, amount);
    Block genesisBlock = block_constructor(GENESIS_DATA, NULL);

    // Only ONE blockchain
    //static Blockchain blockchain = NULL;
    //if (blockchain != NULL)
    //    errx(EXIT_FAILURE, "Error : blockchain already exist");


    // Allocate for blockchain
    Blockchain blockchain = malloc(sizeof(Blockchain));
    if (blockchain == NULL)
        errx(EXIT_FAILURE, "Error : malloc new blockchain");

    // Init blockchain
    blockchain->block = genesisBlock;

    return blockchain;
}

//int blockchain_addBlock(Blockchain blockchain, Transaction transaction)
int blockchain_addBlock(Blockchain blockchain, void *data)
{
    // Create new block with data and add in blockchain
    Block newBlock = block_constructor(data, blockchain->block);

    // Error
    if (newBlock == NULL)
        return 0;

    // Add new block in blockchain
    blockchain->block = newBlock;

    // Block added
    return 1;
}

int blockchain_isValid(Blockchain blockchain)
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
        //char *hash_current = blockchain_hash_str(current->hash);
        //current->nonce = 0;
        //blockchain_init_bufferHash(current, 0);
        //blockchain_block_mining(current);

        //if (hash_current != blockchain_hash_str(current->hash))
        //    return current->index;

        // Check block
        if (block_isValid(current) != 1)
            return current->index;

        // Init string and error
        char *previousHash_Str = buffer_toString(previous->hash);
        char *currentPreviousHash_Str = buffer_toString(current->previousHash);
        //printf("index(%li) previousHash(%s) current->PreviousHash(%s)\n", current->index, previousHash_Str, currentPreviousHash_Str);
        int error_previousHash_currentPreviousHash = strcmp(previousHash_Str, currentPreviousHash_Str);

        // Free string
        free(previousHash_Str);
        free(currentPreviousHash_Str);

        // Error hash previous
        if (error_previousHash_currentPreviousHash > 0)
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

long PoW_findNonce(char *message)
{
    long nonce = 0;

    while (PoW_isValidNonce(message, nonce) != 1)
    {
        //printf("%ld\n", nonce);
        nonce++;
    }
    return nonce;
}

char *PoW_hash(char *message)
{
    Buffer buff = hash_StringToBuffer(message);
    char *hash = buffer_toString(buff);

    // Free bufffer
    buffer_destructor(&buff);

    return hash;
}

int PoW_isValidNonce(char *message, long nonce)
{
    //Hash : message+nonce : string -> buffer -> string
    char *nonce_Str = string_long(nonce);
    char *msg = string_cat(message, nonce_Str);
    char *hash = PoW_hash(msg);

    //printf("hash : %s\n", hash);

    size_t i = 0;
    for (; i < PROOF; i++)
    {
        if (hash[i] != '0')
            break;
    }

    free(nonce_Str);
    free(msg);
    free(hash);

    return i == PROOF;
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
}*/