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
    double amount;
    Buffer signature;
    Buffer date;
};

typedef struct s_transaction *Transaction;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int transaction_constructor(Transaction *p_tran,
                            Buffer from,
                            Buffer to,
                            long amount,
                            Buffer privKey);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void transaction_destructor(Transaction transac);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  OPERATOR                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int transaction_is_valid(Transaction transaction, bool *result);

#endif // UNCOIN__MODULE__TRANSACTION__TRANSACTION_H_