#include "../hash_table.h"

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
        htab_destructor(htab);
        *new_htab = NULL;
        return NO_SPACE;
    }

    // return the htab
    *new_htab = htab;
    // return a success code
    return SUCCESS;
}