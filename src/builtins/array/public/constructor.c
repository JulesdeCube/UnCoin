#include "../array.h"

error_t array_constructor(Array *new_array, typesize_t typesize)
{
    // if there is no return pointer throw an error
    if (new_array == NULL)
        return NO_SELF;

    // no element as a size of 0
    if (typesize == 0)
        return ERROR_VALUE;

    // get the struct malloc
    *new_array = malloc(sizeof(struct s_array));

    // if we can't malloc it return an error
    if (*new_array == NULL)
        return NO_SPACE;

    (*new_array)->typesize = typesize;
    (*new_array)->data = NULL;
    (*new_array)->size = 0;

    return SUCCESS;
}
