#include "../hash_table.h"

void list_clean(Pair list)
{
    Buffer buffer;
    for (Pair pair = list, next_pair; pair;)
    {
        next_pair = pair_get_next(pair);
        buffer = pair_get_hkey(pair);
        buffer_destructor_safe(&buffer);
        buffer = pair_get_key(pair);
        buffer_destructor_safe(&buffer);
        free(pair_get_value(pair));
        free(pair);
        pair = next_pair;
    }
}

void htab_clear(Htab htab)
{
    htab_set_size(htab, 0);
    Pair data = htab_get_pair(htab);
    Pair data_end = data + htab_get_capacity(htab);
    for (; data < data_end; pair_get_next(data), data++)
        list_clean(pair_get_next(data));
}

int htab_destructor(Htab htab)
{
    if (htab == NULL)
        return NO_SELF;
    htab_clear(htab);
    free(htab_get_pair(htab));
    free(htab);
    return SUCCESS;
}
