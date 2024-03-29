#ifndef UNCOIN__MODULE__HASH__HASH_H_
#define UNCOIN__MODULE__HASH__HASH_H_

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
#include "utils/type.h"
#include "builtins/buffer/buffer.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 FUNCTIONS                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Hash a buffer
 *
 * \param buff_result the buffer resulting of the hash function.
 * \param buff_to_hash the buffer to be hash.
 *
 * \return the error occur
 */
error_t hash(Buffer *buff_result, Buffer *buff_to_hash);

#endif // UNCOIN__MODULE__HASH__HASH_H_