#include "../hash_table.h"

void _htab_list_clean(_Pair list, Destructor destructor);

void htab_clear(Htab htab, Destructor destructor)
{
    _Pair data = htab->data;
    size_t size = htab->capacity;

    for (; size; --size, ++data)
        _htab_list_clean(data, destructor);
}

void htab_destructor(Htab htab, Destructor destructor)
{
    if (htab == NULL)
        return;

    if (htab->data != NULL)
    {
        htab_clear(htab, destructor);
        free(htab->data);
    }

    free(htab);
}
