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

struct s_bigint
{
    size_t exhibitor;
    bool sign;
    Buffer buffer;
};

typedef struct s_bigint *BigInt;

#define POSITIVE true
#define NEGATIVE false

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int bigint_constructor_array(BigInt *new_bigint, bool sign, size_t size, unsigned char *array);
int bigint_constructor_null(BigInt *new_bigint);
int bigint_constructor_buffer(BigInt *new_bigint, bool sign, Buffer buffer);
int bigint_constructor_buffer_signed(BigInt *new_bigint, Buffer buffer);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

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
