#include "../array.h"

error_t array_foreach(Array array, operator_t operation, any_t args)
{
    // check if there is an array
    if (array == NULL)
        return NO_SELF;

    // if there is no operation to do just quite
    if (operation == NULL)
        return SUCCESS;

    // loop over all the element and apply the function
    data_t *data = array->data;
    size_t size = array_get_size(array);
    typesize_t typesize = array_get_typesize(array);
    for (; size; --size)
    {
        operation(data, args);
        // skip the next element
        data += typesize;
    }

    return SUCCESS;
}

error_t array_get_i(Array array, size_t i, any_t *result)
{
    // check if there is an array
    if (array == NULL)
        return NO_SELF;

    // check if the return pointer exist
    if (result == NULL)
        return ERROR_VALUE;

    // check if not out of range
    if (i >= array_get_size(array))
        return OUT_OF_RANGE;

    // calculate the index of the element
    *result = array->data + i * array_get_typesize(array);

    return SUCCESS;
}

error_t array_set_i(Array array, size_t i, any_t value)
{
    // check if there return pointer
    if (value == NULL)
        return ERROR_VALUE;

    // get the pointer to the wanted element
    any_t data;
    TRY(array_get_i(array, i, &data));

    // copy the value
    memcpy(data, value, array_get_typesize(array));

    return SUCCESS;
}

error_t array_push(Array array, any_t element)
{
    // check if there is an array
    if (array == NULL)
        return NO_SELF;

    // realocate the array
    data_t *new_data = realloc(array->data, (array->size + 1) * array->typesize);
    if (new_data == NULL)
        return NO_SPACE;
    array->data = new_data;

    // calculate the element place
    data_t *place = new_data + array->typesize * array->size++;

    data_t *e = (data_t *)element;

    // copy the value
    memcpy(place, e, array->typesize);

    return SUCCESS;
}