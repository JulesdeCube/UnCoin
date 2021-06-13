#include "../buffer.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PRIVATE DEFINTIONS                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \private
**
** \brief destroy the data of the buffer.
** if safe is true all the data will be errase
*/
void _buffer_destructor_data(Buffer buffer, bool_t safe);

/**
** \private
**
** \brief destroy the buffer. Unsafe
** if safe is true all the data will be errase
*/
void _buffer_destructor_buffer(Buffer buffer, bool_t safe);

/**
** \private
**
** \brief destroy the buffer.
** if safe is true all the data will be errase
*/
void _buffer_destructor(Buffer *buffer, bool_t safe);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                            PUBLIC IMPLEMENTATION                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void buffer_destructor(Buffer *buffer)
{
    // just destroy
    _buffer_destructor(buffer, false);
}

void buffer_destructor_safe(Buffer *buffer)
{
    // destroy and clean
    _buffer_destructor(buffer, true);
}