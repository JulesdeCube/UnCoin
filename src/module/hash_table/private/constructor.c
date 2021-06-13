#include "../hash_table.h"

int _htab_constructor_pair(_Pair *pair, Buffer key, void *value, Buffer *hkey)
{
    int error = hash(hkey, &key);
    if (error != SUCCESS)
        return error;

    _Pair new_pair = malloc(sizeof(struct s_pair));
    if (new_pair == NULL)
        return NO_SPACE;

    new_pair->key = key;
    new_pair->hkey = *hkey;
    new_pair->value = value;

    // return with using the pointer
    *pair = new_pair;

    return SUCCESS;
}