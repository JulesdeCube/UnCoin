#include "../buffer.h"

error_t buffer_constructor_buffer(Buffer *new_buffer, Buffer buffer)
{
    // if there is no return pointer throw an error
    if (new_buffer == NULL)
        return NO_SELF;

    // if there is no buffer parse as parameter return an error
    if (buffer == NULL)
    {
        *new_buffer = NULL;
        return ERROR_VALUE;
    }

    u_char *data = buffer_get_data(buffer);
    if (data == NULL)
    {
        *new_buffer = NULL;
        return INTERNAL_ERROR;
    }

    // contruct the buffer with the array constructor
    return buffer_constructor_array(new_buffer,
                                    buffer_get_size(buffer),
                                    data);
}

error_t buffer_constructor_size(Buffer *new_buffer, size_t size)
{
    // if there is no return pointer throw an error
    if (new_buffer == NULL)
        return NO_SELF;

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
        *new_buffer = NULL;
        return NO_SPACE;
    }

    // return the buffer
    *new_buffer = buffer;
    // return a success code
    return SUCCESS;
}

error_t buffer_constructor_const(Buffer *new_buffer, size_t size, u_char constant)
{
    // create a buffer of the correct size
    TRY(buffer_constructor_size(new_buffer, size));

    // fill all the buffer with the coresponding constant
    memset(buffer_get_data(*new_buffer), constant, size);

    // return a success code
    return SUCCESS;
}

error_t buffer_constructor_array(Buffer *new_buffer, size_t size, u_char *array)
{
    // if there is no return pointer throw an error
    if (new_buffer == NULL)
        return NO_SELF;

    // if the array is null do nothing
    if (array == NULL)
    {
        *new_buffer = NULL;
        return ERROR_VALUE;
    }

    // get a new buffer
    TRY(buffer_constructor_size(new_buffer, size));

    // fill the buffer with the values
    memcpy(buffer_get_data(*new_buffer), array, size);

    // return a success code
    return SUCCESS;
}

error_t buffer_constructor_str(Buffer *new_buffer, char *str, bool_t strict)
{
    // if there is no return pointer throw an error
    if (new_buffer == NULL)
        return NO_SELF;

    // if the string is null do nothing
    if (str == NULL)
    {
        *new_buffer = NULL;
        return ERROR_VALUE;
    }
    // create a array buffer with the correct lenght
    return buffer_constructor_array(new_buffer,
                                    strlen(str) + (strict != false),
                                    (u_char *)str);
}
