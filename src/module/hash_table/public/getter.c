#include "../hash_table.h"

size_t htab_get_size(Htab htab)
{
    return htab == NULL ? 0 : htab->size;
}

size_t htab_get_capacity(Htab htab)
{
    return htab == NULL ? 0 : htab->capacity;
}

Pair htab_get_pair(Htab htab)
{
    return htab == NULL ? 0 : htab->data;
}

size_t htab_get_ratio(Htab htab)
{
    return 100ul * htab->size / htab->capacity;
}

Buffer pair_get_hkey(Pair pair)
{
    return pair == NULL ? 0 : pair->hkey;
}

Buffer pair_get_key(Pair pair)
{
    return pair == NULL ? 0 : pair->key;
}

void *pair_get_value(Pair pair)
{
    return pair == NULL ? 0 : pair->value;
}

Pair pair_get_next(Pair pair)
{
    return pair == NULL ? 0 : pair->next;
}