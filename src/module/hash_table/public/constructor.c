#include "../hash_table.h"

int construct_pair(Pair *pair, Buffer key, void *value, Buffer *hkey)
{
    int error = hash(hkey, &key);
    if (error != SUCCESS)
        return error;

    Pair new_pair = malloc(sizeof(struct s_pair));
    if (new_pair == NULL)
        return NO_SPACE;

    new_pair->key = key;
    new_pair->hkey = *hkey;
    new_pair->value = value;

    // return with using the pointer
    *pair = new_pair;

    return SUCCESS;
}

int htab_constructor(Htab *new_htab)
{
    // get the struct malloc
    Htab htab = malloc(sizeof(struct s_htab));

    // if we can't malloc it return an error
    if (htab == NULL)
        return NO_SPACE;

    // initialize the porperties
    htab->capacity = DEFAULT_CAPACITY;
    htab->size = 0;
    htab->data = calloc(htab->capacity, sizeof(struct s_pair));

    // get check if the data is allocated
    if (htab->data == NULL)
    {
        // destroy all an return error
        htab_destructor(htab, NULL);
        *new_htab = NULL;
        return NO_SPACE;
    }

    // return the htab
    *new_htab = htab;
    // return a success code
    return SUCCESS;
}