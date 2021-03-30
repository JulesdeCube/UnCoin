#ifndef UNCOIN__UILTINS_BIGINT__BIGINT_H_
#define UNCOIN__UILTINS_BIGINT__BIGINT_H_

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  LIBRARY                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "builtins/buffer/buffer.h"

#include "utils/error.h"
#include "utils/bool.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              TYPE DEFINTIONS                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
* \struct s_bigint
* \brief a inter with variable size
*
* compose of an exhibitor the sign of the number and a buffer that repesent the
* number with the less signifcative bit on the right (end of the buffer).
*/
struct s_bigint
{
    size_t exhibitor;
    bool sign;
    Buffer buffer;
};

/**
** \typedef BigInt
** \brief a big int
*/
typedef struct s_bigint *BigInt;

// Positive sign flag
#define POSITIVE false
// Negative sign flag
#define NEGATIVE true

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// !TODO - need comment

int bigint_constructor_array(BigInt *new_bigint, bool sign, size_t size,
                             u_char *array);
int bigint_constructor_null(BigInt *new_bigint);
int bigint_constructor_buffer(BigInt *new_bigint, bool sign, Buffer buffer);
int bigint_constructor_buffer_signed(BigInt *new_bigint, Buffer buffer);
int bigint_constructor_bigint(BigInt *new_bigint, BigInt bigint);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// !TODO - need comment

void bigint_destructor(BigInt *bigint);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               GETTER/SETTER                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 OPERATION                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#endif // UNCOIN__UILTINS_BIGINT__BIGINT_H_
