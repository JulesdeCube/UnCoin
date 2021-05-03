#ifndef UNCOIN__UILTINS_BIGINT__BIGINT_H_
#define UNCOIN__UILTINS_BIGINT__BIGINT_H_

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  LIBRARY                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <stdio.h>
#include <limits.h>
#include <sys/types.h>

#include "builtins/buffer/buffer.h"

#include "utils/error.h"
#include "utils/bool.h"
#include "utils/math.h"

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
int bigint_constructor_from_int(BigInt *new_bigint, int value);

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

bool bigint_is_null(BigInt bigint);
size_t bigint_get_exhibitor(BigInt bigint);
int bigint_get_byte(BigInt bigint, size_t i, u_char *byte);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 OPERATION                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int bigint_addition(BigInt bigint1, BigInt bigint2, BigInt *result);
int bigint_substraction(BigInt bigint1, BigInt bigint2, BigInt *result);
int bigint_left_shift(BigInt bigint, size_t shift, BigInt *result);
int bigint_right_shift(BigInt bigint, size_t shift, BigInt *result);
int bigint_shift(BigInt bigint, ssize_t shift, BigInt *result);
int bigint_multiplication(BigInt bigint1,BigInt bigint2,BigInt *result);

////////////////////////////////////////////////////////////
//                       CONVERSION                       //
////////////////////////////////////////////////////////////

int bigint_to_bool(BigInt bigint, bool *result);
int bigint_to_int(BigInt bigint, int *result);
long long int bigint_to_long_long_int(BigInt bigint, long long int *result);
int bigint_to_buffer(BigInt bigint, Buffer *buffer);
int bigint_to_string(BigInt bigint, char **str, size_t *len);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 COMPARAISON                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool bigint_greater_than(BigInt bigint1, BigInt bigint2);
bool bigint_less_than(BigInt bigint1, BigInt bigint2);
bool bigint_equal_than(BigInt bigint1, BigInt bigint2);
bool bigint_not_equal(BigInt bigint1, BigInt bigint2);
bool bigint_less_or_equal(BigInt bigint1, BigInt bigint2);
bool bigint_greater_or_equal(BigInt bigint1, BigInt bigint2);

#endif // UNCOIN__UILTINS_BIGINT__BIGINT_H_
