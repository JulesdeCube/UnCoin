#ifndef UNCOIN__LIBS_BUILTINS_BUFFER__BUFFER_H_
#define UNCOIN__LIBS_BUILTINS_BUFFER__BUFFER_H_

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  LIBRARY                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>

#include "utils/error.h"
#include "utils/bool.h"
//#include ""

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              TYPE DEFINTIONS                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

struct s_BigInt
{
    size_t exhibitor;
    Buffer buffer;
    Sign is_positive;
}
typedef struct s_BigInt *BigInt;
typedef int Buffer;
typedef bool Sign;
#define POSITIVE true
#define NEGATIVE false

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int buffer_constructor_buffer(Buffer *new_buffer, Buffer buffer);


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void buffer_destructor(Buffer *buffer);


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               GETTER/SETTER                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

size_t buffer_get_size(Buffer buffer);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 OPERATION                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int buffer_get_index(Buffer buffer, size_t index, char *byte);

#endif // UNCOIN__LIBS_BUILTINS_BUFFER__BUFFER_H_
