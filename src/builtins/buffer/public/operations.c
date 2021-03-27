#include "../buffer.h"

int buffer_get_index(Buffer buffer, size_t index, char *byte)
{
    // if no buffer return an error
    if (buffer == NULL)
        return NO_SELF;

    // if out of range return error
    if (index > buffer_get_size(buffer))
        return OUT_OF_RANGE;

    // get data
    char *data = buffer_get_data(buffer);
    // wong modifications
    if (data == NULL)
        return INTERNAL_ERROR;

    // update the output
    *byte = data[index];
    // return success code
    return SUCCESS;
}
