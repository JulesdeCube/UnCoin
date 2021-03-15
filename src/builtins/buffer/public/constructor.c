#include "../buffer.h"

int buffer_constructor_buffer(Buffer *new_buffer, Buffer buffer)
{
    // if there is no buffer parse as parameter return an error
    if (buffer == NULL)
    {
        *new_buffer = NULL;
        return NO_SELF;
    }

    // contruct the buffer with the array constructor
    return buffer_constructor_array(new_buffer,
                                    buffer_get_size(buffer),
                                    buffer_get_data(buffer));
}

int buffer_constructor_size(Buffer *new_buffer, size_t size)
{
    // get the struct malloc
    Buffer buffer = malloc(sizeof(struct s_buffer));

    // if we can't malloc it return an error
    if (buffer == NULL)
        return NO_SPACE;

    // initialize the porperties
    buffer->size = size;
    buffer->data = malloc(size);

    // get check if the data is allocated
    if (buffer->data == NULL)
    {
        // destoy all and return error
        buffer_destructor(&buffer);
        return NO_SPACE;
    }

    // return the buffer
    *new_buffer = buffer;
    // return a success code
    return SUCCESS;
}

int buffer_constructor_const(Buffer *new_buffer, size_t size, char constant)
{
    // create a buffer of the correct size
    int error = buffer_constructor_size(new_buffer, size);

    // if there is an error stop and retur the error
    if (error != SUCCESS)
        return error;

    // fill all the buffer with the coresponding constant
    memset(buffer_get_data(*new_buffer), constant, size);

    // return a success code
    return SUCCESS;
}

int buffer_constructor_array(Buffer *new_buffer, size_t size, char *array)
{
    // if the array is null do nothing
    if (array == NULL)
    {
        *new_buffer = NULL;
        return NO_SELF;
    }

    // get a new buffer
    int error = buffer_constructor_size(new_buffer, size);

    // if there is an error stop and retur the error
    if (error != SUCCESS)
        return error;

    // fill the
    memcpy(buffer_get_data(*new_buffer), array, size);

    // return a success code
    return SUCCESS;
}

int buffer_constructor_str(Buffer *new_buffer, char *str, bool strict)
{
    // if the string is null do nothing
    if (str == NULL)
    {
        *new_buffer = NULL;
        return NO_SELF;
    }
    // create a array buffer with the correct lenght
    return buffer_constructor_array(new_buffer,
                                    strlen(str) + (strict != false),
                                    str);
}
