#include "../hash_table.h"

size_t htab_get_size(Htab htab)
{
    return htab == NULL ? 0 : htab->size;
}

size_t htab_get_capacity(Htab htab)
{
    return htab == NULL ? 0 : htab->capacity;
}

size_t htab_get_ratio(Htab htab)
{
    return 100ul * htab->size / htab->capacity;
}