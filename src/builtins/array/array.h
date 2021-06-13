#ifndef UNCOIN__UILTINS_ARRAY__ARRAY_H_
#define UNCOIN__UILTINS_ARRAY__ARRAY_H_

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  LIBRARY                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "utils/error.h"
#include "utils/type.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              TYPE DEFINTIONS                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \typedef typesize_t
** \brief memory size of a type
*/
typedef size_t typesize_t;

/**
** \typedef data_t
** \brief data
*/
typedef char data_t;

/**
* \struct s_array
* \brief a dynamic array
*
* containe the data the number of element and the typesize
*/
struct s_array
{
    data_t *data;
    size_t size;
    typesize_t typesize;
};

/**
** \typedef Array
** \brief A dynamic array
*/
typedef struct s_array *Array;

/**
** \typedef operator_t
** \brief a operator
**
** fist parameter: the element it self
** second parameter: any argumement
*/
typedef void (*operator_t)(any_t, any_t);

/**
** \typedef destuctor_t
** \brief a destructor
**
** parameter: the element to destroy
*/
typedef void (*destructor_t)(any_t);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

error_t array_constructor(Array *new_array, typesize_t typesize);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void array_destructor(Array *array, destructor_t destructor);

void array_destructor_safe(Array *array, destructor_t destructor);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               GETTER/SETTER                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

size_t array_get_size(Array array);

typesize_t array_get_typesize(Array array);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  OPERATOR                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

error_t array_get_i(Array array, size_t i, any_t *result);

error_t array_set_i(Array array, size_t i, any_t value);

error_t array_push(Array array, any_t element);

error_t array_foreach(Array array, operator_t operation, any_t args);

#endif // UNCOIN__UILTINS_ARRAY__ARRAY_H_