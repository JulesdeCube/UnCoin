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
#include "constructor.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void buffer_destructor(Buffer *buffer);


#endif // UNCOIN__LIBS_BUILTINS_BUFFER__BUFFER_H_
