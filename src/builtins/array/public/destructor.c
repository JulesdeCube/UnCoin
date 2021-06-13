#include "../array.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PRIVATE DEFINTIONS                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void _array_opt_to_dest(any_t e, any_t destructor)
{
    if (destructor == NULL)
        return;

    destructor_t dest = destructor;
    dest(e);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                            PUBLIC IMPLEMENTATION                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void array_destructor(Array *array, destructor_t destructor)
{
    // check for null pointer
    if (array == NULL || *array == NULL)
        return;

    // get the array
    Array arr = *array;

    // destroy each element
    array_foreach(arr, _array_opt_to_dest, destructor);

    // free the data and the array it self
    free(arr->data);
    free(arr);

    // set pointer to null
    *array = NULL;
}

void array_destructor_safe(Array *array, destructor_t destructor)
{
    // check for null pointer
    if (array == NULL || *array == NULL)
        return;

    // get the array
    Array arr = *array;

    // if there is data set to null
    if (arr->data != NULL)
        memset((*array)->data, 0, array_get_size(arr));

    // destroy the array
    array_destructor(array, destructor);
}