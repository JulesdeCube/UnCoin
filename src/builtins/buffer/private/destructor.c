#include "../buffer.h"

void _buffer_destructor_data(Buffer buffer, bool safe)
{
    // get the data
    u_char *data = buffer_get_data(buffer);

    // if no data do nothing
    if (data == NULL)
        return;

    // clean the buffer if in safe mode
    if (safe)
        memset(data, 0, buffer_get_size(buffer));

    // free the data
    free(data);
}

void _buffer_destructor_buffer(Buffer buffer, bool safe)
{
    // if already destoy do nothing
    if (buffer == NULL)
        return;

    // destroy the data
    _buffer_destructor_data(buffer, safe);

    // if safe mode clean the datas
    if (safe)
        memset(buffer, 0, sizeof(struct s_buffer));

    // free the buffer
    free(buffer);
}

void _buffer_destructor(Buffer *buffer, bool safe)
{
    // if no buffer given do noting
    if (buffer == NULL)
        return;

    // destroy the buffer
    _buffer_destructor_buffer(*buffer, safe);

    // set to null
    *buffer = NULL;
}