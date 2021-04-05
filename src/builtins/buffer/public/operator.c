#include "../buffer.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PRIVATE DEFINTIONS                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \private
**
** \param buffer buffer to get the pointer
** \param  index index of the byte
** \param   byte return pointer for the pointer
**
** \return error code
**
** \throw        NO_SELF: if buffer is null
** \throw   OUT_OF_RANGE: the index is out of range
** \throw INTERNAL_ERROR: no data
*/
int _buffer_get_index_pointer(Buffer buffer, size_t index, u_char **byte);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                            PUBLIC IMPLEMENTATION                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int buffer_get_index(Buffer buffer, size_t index, u_char *byte)
{
    u_char *ptr;
    // get the coresponding pointer
    int error = _buffer_get_index_pointer(buffer, index, &ptr);

    // if we can't get the pointer
    if (error != SUCCESS)
        return error;

    // return the value
    *byte = *ptr;
    // return success code
    return SUCCESS;
}

int buffer_set_index(Buffer buffer, size_t index, u_char byte)
{
    u_char *ptr;
    // get the coresponding pointer
    int error = _buffer_get_index_pointer(buffer, index, &ptr);

    if (error != SUCCESS)
        return error;

    // update the value
    *ptr = byte;
    // return success code
    return SUCCESS;
}

int buffer_to_hex(Buffer buffer, char **str, size_t *len)
{
    // set paramerer to default value
    if (str)
        *str = NULL;
    if (len)
        *len = 0;

    // if there is a no buffer it's an error
    if (buffer == NULL)
        // return a nerror
        return NO_SELF;

    // get the size of both elements
    size_t buff_size = buffer_get_size(buffer);
    size_t str_size = buff_size << 1;

    // if len parameter is definit set it
    if (len != NULL)
        *len = str_size;

    // if str is not set just return the lenght
    if (str == NULL)
        return SUCCESS;

    // create the buffer (the len of the string + the null caracter)
    *str = malloc(str_size + 1);

    // if error durring malloc return no space
    if (*str == NULL)
        return NO_SPACE;

    u_char *start = buffer_get_data(buffer);
    u_char *end = start + buff_size - 1;
    char *c = *str;
    while (start <= end)
    {
        *c++ = int_to_hex[(*end & 0xf0) >> 4];
        *c++ = int_to_hex[*end-- & 0x0f];
    }

    *c = '\00';
    return SUCCESS;
}