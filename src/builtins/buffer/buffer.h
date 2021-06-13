#ifndef UNCOIN__BUILTINS_BUFFER__BUFFER_H_
#define UNCOIN__BUILTINS_BUFFER__BUFFER_H_

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  LIBRARY                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>

#include "utils/error.h"
#include "utils/bool.h"
#include "utils/hex.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              TYPE DEFINTIONS                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

typedef unsigned char u_char;

/**
** \struct s_buffer
** \brief a buffer structure
**
** compose of the data and it's corresponding size
*/
struct s_buffer
{
    size_t size;
    u_char *data;
};

/**
** \typedef Buffer
** \brief a buffer
*/
typedef struct s_buffer *Buffer;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \brief `Buffer` duplication constuctor.
**
** ⚠️** you need to use the buffer destructor function after used (to free space
** )** ⚠️
**
** \param new_buffer a pointer to the output buffer
** \param     buffer the buffer to copy
**
** \return error code
**
** \throw ERROR_VALUE: if buffer is null
** \see `buffer_constructor_array` for other error code
*/
error_t buffer_constructor_buffer(Buffer *new_buffer, Buffer buffer);

/**
** \brief `Buffer` constuctor.
**
** Create a buffer of `size` size. ⚠️ buffer is initialize with random values.
**
** ⚠️** you need to use the buffer destructor function after used (to free space
** )** ⚠️
**
** \param new_buffer a pointer to the output buffer
** \param       size the size of the buffer
**
** \return error code
**
** \throw NO_SPACE: not enough free space
** \throw NO_SELF : return pointer (`new_buffer`) is null
*/
error_t buffer_constructor_size(Buffer *new_buffer, size_t size);

/**
** \brief `Buffer` constant constuctor.
**
** Create a buffer of `size` time the `constant` value.
**
** ⚠️** you need to use the buffer destructor function after used (to free space
** )** ⚠️
**
** \param new_buffer a pointer to the output buffer
** \param       size the size of the buffer
** \param   constant the constant
**
** \return error code
**
** \see `buffer_constructor_size` for other error code
*/
error_t buffer_constructor_const(Buffer *new_buffer, size_t size,
                                 u_char constant);

/**
** \brief `Buffer` array constuctor.
**
** Create a buffer from an array and it's size.
**
** ⚠️** you need to use the buffer destructor function after used (to free space
** )** ⚠️
**
** \param new_buffer a pointer to the output buffer
** \param       size the size of the buffer
** \param      array the array to duplique
**
** \return error code
**
** \throw ERROR_VALUE: if array is null
** \see `buffer_constructor_size` for other error code
*/
error_t buffer_constructor_array(Buffer *new_buffer, size_t size,
                                 u_char *array);

/**
** \brief `Buffer` string constuctor.
**
** Create a buffer from a string.
**
** ⚠️** you need to use the buffer destructor function after used (to free space
** )** ⚠️
**
** \param new_buffer a pointer to the output buffer
** \param        str the string to copy
** \param     strict add or not the ending null byte
**
** \return error code
**
** \throw ERROR_VALUE: if string is null
** \see `buffer_constructor_array` for other error code
*/
error_t buffer_constructor_str(Buffer *new_buffer, char *str, bool strict);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \brief `Buffer` destructor.
**
**  free space and propely destroy the buffer.
**
** \param buffer a pointer to the buffer
*/
void buffer_destructor(Buffer *buffer);

/**
** \brief `Buffer` destructor.
**
**  free space, propely destroy the buffer and clean the data (set to 0).
**
** \param buffer a pointer to the buffer
*/
void buffer_destructor_safe(Buffer *buffer);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               GETTER/SETTER                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \brief get the size of the buffer
**
** \param buffer the buffer
**
** \return the size of the buffer or 0 if the buffer is null
*/
size_t buffer_get_size(Buffer buffer);

/**
** \brief get the size of the buffer
**
** \param buffer the buffer
**
** \return the size of the data pointer or NULL if the buffer is null
*/
u_char *buffer_get_data(Buffer buffer);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  OPERATOR                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \brief get the value at the given index
**
** \param buffer the buffer to read
** \param  index the index of byte in buffer
** \param   byte pointer to return the value
**
** \return error code
**
** \throw NO_SELF       : if no buffer given
** \throw OUT_OF_RANGE  : if the index is greater than the size
** \throw INTERNAL_ERROR: data is null
*/
error_t buffer_get_index(Buffer buffer, size_t index, u_char *byte);

error_t buffer_set_index(Buffer buffer, size_t index, u_char byte);

error_t buffer_to_hex(Buffer buffer, char **str, size_t *len);

#endif // UNCOIN__BUILTINS_BUFFER__BUFFER_H_