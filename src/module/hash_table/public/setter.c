#include "../hash_table.h"

void htab_set_size(Htab htab, size_t size)
{
    htab->size = size;
}