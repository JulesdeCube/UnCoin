#ifndef UNCOIN__BUILTINS_MODULE_HASH_TESTS__HASH_TOOLS_H_
#define UNCOIN__BUILTINS_MODULE_HASH_TESTS__HASH_TOOLS_H_

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  LIBRARY                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include "../hash.h"
#include "test/test.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              TYPE DEFINITONS                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define FALSE 0
#define TRUE !FALSE

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 FUNCTIONS                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief return the higher between to size_t
 *
 * \param  first argument
 * \param second argument
 *
 * \return the higher between both
 */
size_t max_size_t(size_t first, size_t second);

/**
 * \brief Create a buffer and hash it
 *
 * \param buff_result the buff resulting of the hash function.
 * \param         str string to put in a buffer and to hash
 *
 * \return the buff before hashing
 */
Buffer *create_buff_hashed(Buffer *buff_result, char *str);

#endif // UNCOIN__BUILTINS_MODULE_HASH_TESTS__HASH_TOOLS_H_