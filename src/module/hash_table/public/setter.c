#include "../hash_table.h"

void htab_set_size(Htab htab, size_t size)
{
    htab->size = size;
}

void htab_set_capacity(Htab htab, size_t new_capacity)
{
    htab->capacity = new_capacity;
}