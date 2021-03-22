#include "../buffer.h"

size_t buffer_get_size(Buffer buffer)
{
    return buffer == NULL ? 0 : buffer->size;
}

char *buffer_get_data(Buffer buffer)
{
    return buffer == NULL ? 0 : buffer->data;
}