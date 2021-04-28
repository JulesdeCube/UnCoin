#include "../hash_table.h"

void list_clean(Pair list, Destructor destructor)
{
    Buffer buffer;
    for (Pair pair = list, next_pair; pair != NULL;)
    {
        next_pair = pair_get_next(pair);
        buffer = pair_get_hkey(pair);
        buffer_destructor_safe(&buffer);
        buffer = pair_get_key(pair);
        buffer_destructor_safe(&buffer);

        // If there is a destructor function call it
        if (destructor != NULL)
            destructor(pair->value);

        free(pair);
        pair = next_pair;
    }
}

void htab_clear(Htab htab, Destructor destructor)
{
    Pair data = htab_get_pair(htab);
    Pair data_end = data + htab_get_capacity(htab);
    for (; data < data_end; pair_get_next(data), data++)
    {
        list_clean(pair_get_next(data), destructor);
    }

}

int htab_destructor(Htab htab, Destructor destructor)
{
    if (htab == NULL)
        return NO_SELF;
    htab_clear(htab, destructor);
    free(htab_get_pair(htab));
    free(htab);
    return SUCCESS;
}