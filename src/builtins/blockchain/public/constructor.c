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

Buffer block_to_buffer(Block block)
{
    Buffer b_transaction;
    transaction_to_buffer(block->transaction, &b_transaction);
    size_t size = sizeof(size_t) * 3 + sizeof(long) + buffer_get_size(block->previousHash) + buffer_get_size(b_transaction);
    Buffer buff;
    buffer_constructor_size(&buff,size);

    u_char *data = buffer_get_data(*buff);
    _copy_buffer(&data,block->previousHash);
    _copy_buffer(&data,b_transaction);

    size_t *tmp = data;
    *tmp = block->index;
    data+=sizeof(size_t)

    long *nounce = data;
    *nounce = block->nonce;
    data+=sizeof(long);

    buffer_destructor_safe(&b_transaction);
    return buff;
}

void blockchain_block_hash(Block block)
{
    if (block->hash != NULL)
        buffer_destructor(&block->hash);
    Buffer b_buff = block_to_buffer(block);
    hash(block->hash,b_buff);
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

Block blockchain_block_constructor(Transaction transaction, Block lastBlock)
{
    // Allocate for block
    Block block = calloc(1, sizeof(Block));
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
    block->transaction = transaction;
    /*
    printf("%s\n",(char*)buffer_get_data(block->transaction->from));
    printf("%s\n",(char*)buffer_get_data(block->transaction->to));*/
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
    Buffer from;
    Buffer to;
    long long unsigned int amount = 0;
    time_t date;
    time(&date);

    buffer_constructor_str(&from, "Vincent", true);
    buffer_constructor_str(&to, "Jeanne", true);

    Transaction genesis_transaction;
    transaction_constructor_client(&genesis_transaction, from, to, amount, date);

    // Create genesis block (first block)
    Block genesisBlock = blockchain_block_constructor(genesis_transaction, NULL);
    //printf("oui\n");
    // Allocate for blockchain
    Blockchain blockchain = malloc(sizeof(Blockchain));
    if (blockchain == NULL)
        errx(EXIT_FAILURE, "Error : malloc new blockchain");

    // Init blockchain
    blockchain->block = genesisBlock;

    return blockchain;
}

void blockchain_block_add(Blockchain blockchain, Transaction transaction)
{
    // Create new block with data and add in blockchain
    Block newBlock = blockchain_block_constructor(transaction, blockchain->block);

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
    //printf("oui\n");
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
    size_t i = 0;
    while (i < PROOF && hash[i] == '0')
        i++;

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
    buffer_destructor(&block->hash);
    transaction_destructor(block->transaction);
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