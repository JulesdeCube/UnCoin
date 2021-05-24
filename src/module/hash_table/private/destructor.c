#include "../hash_table.h"

void _htab_pair_destructor(_Pair pair, Destructor destructor)
{
    if (pair == NULL)
        return;

    buffer_destructor_safe(&pair->hkey);
    buffer_destructor_safe(&pair->key);

    // If there is a destructor function call it
    if (destructor != NULL)
        destructor(pair->value);

    free(pair);
}

void _htab_list_clean(_Pair list, Destructor destructor)
{
    _Pair next_pair;
    _Pair pair = list->next;

    while (pair != NULL)
    {
        next_pair = pair->next;
        _htab_pair_destructor(pair, destructor);
        pair = next_pair;
    }
}