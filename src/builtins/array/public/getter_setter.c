#include "../array.h"

size_t array_get_size(Array array)
{
    return array != NULL ? array->size : 0;
}

typesize_t array_get_typesize(Array array)
{
    return array != NULL ? array->typesize : 1;
}
