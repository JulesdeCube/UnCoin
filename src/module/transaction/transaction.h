#ifndef UNCOIN__MODULE__TRANSACTION__TRANSACTION_H_
#define UNCOIN__MODULE__TRANSACTION__TRANSACTION_H_

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  LIBRARY                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "utils/error.h"
#include "utils/bool.h"
#include "builtins/buffer/buffer.h"
#include "../hash_table/hash_table.h"


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             TYPE DEFINITIONS                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

struct s_transaction
{
    Buffer from; // Key public of me
    Buffer to;   // Key public of him
    long long unsigned int amount;
    Buffer message;
    time_t date;
};

typedef struct s_transaction *Transaction;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int transaction_constructor_client(Transaction *p_tran,
                            Buffer from,
                            Buffer to,
                            long long unsigned int amount,
                            time_t time);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void transaction_destructor(Transaction transac);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  GETTER                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

size_t get_transaction_size(Transaction t);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  OPERATOR                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//int transaction_is_valid(Transaction transaction, bool *result);

int transaction_to_buffer(Transaction t, Buffer *buf_result);

int transaction_from_buffer(Transaction *t, Buffer *buf);

int buf_trans_encrypt(Buffer *buf_to_encrypt, Buffer private_key, Buffer *result);

int buf_trans_decrypt(Buffer *buf_to_decrypt, Buffer public_key, Buffer *result);
#endif // UNCOIN__MODULE__TRANSACTION__TRANSACTION_H_
