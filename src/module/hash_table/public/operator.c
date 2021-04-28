#include "../hash_table.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PRIVATE DEFINTIONS                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \private
**
** \brief
*/
Pair _get_pair_with_hashkey(Pair pair, Buffer hkey);

/**
** \private
**
** \brief
*/
Pair __pair_get_list_from_hkey(Pair pair, Buffer hkey, size_t capacity);

/**
** \private
**
** \brief
*/
void __pairlist_append_element(Pair pair_list, Pair element);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PUBLIC IMPLEMENTATIONS                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int htab_get_corresponding_pair(Htab htab, Pair pair_to_test, Pair *pair_result)
{
    // If htab is null or contains nothing
    if (htab == NULL || htab_get_size(htab) == 0)
        return NO_SELF;

    // get the list of the possible position
    Pair where_can_be_list = __pair_get_list_from_hkey(htab->data,
                                            pair_to_test->hkey,
                                            htab->capacity);

    // Check if an element in the list has the same hash key
    Pair where_can_be_pair = _get_pair_with_hashkey(where_can_be_list->next,
                                                    pair_to_test->hkey);

    // element not exist
    if (where_can_be_pair == NULL)
        return OUT_OF_RANGE;

    // test if it is not 2 different key with same hkey
    char *str1 = (char *) pair_to_test->hkey->data;
    char *str2 = (char *) where_can_be_pair->hkey->data;
    if(strcmp(str1, str2))
        return OUT_OF_RANGE;

    //we are good now
    *pair_result = where_can_be_pair;
    return SUCCESS;
}

int print_pairs(Pair pair)
{
    Buffer hkey, key;
    void *value;
    char *s_hkey;
    u_char *s_key;

    while (pair != NULL)
    {
        // get informations about current pair
        hkey = pair_get_hkey(pair);
        key = pair_get_key(pair);
        value = pair_get_value(pair);

        buffer_to_hex(hkey, &s_hkey, NULL);
        s_key = buffer_get_data(key);

        printf(" -> (%s, %s, %s)", s_hkey, (char *)s_key, (char *)value);

        free(s_hkey);
        // take the next pair.
        pair = pair_get_next(pair);
    }
    return SUCCESS;
}

void print_htab(Htab htab)
{
    if (htab == NULL)
    {
        printf("Htab is Null\n");
        return;
    }
    size_t capacity, size, ratio;
    // Get and print informations about our hash table
    capacity = htab_get_capacity(htab);
    size = htab_get_size(htab);
    ratio = 100 * size / capacity;
    printf("(capacity, size, ratio) = (%ld, %ld, %ld)\n",
           capacity,
           size,
           ratio);

    Pair pair;
    // Get the first pair
    Pair htab_pair = htab_get_pair(htab);
    // run and print on all pairs
    for (size_t i = 0; i < capacity; ++i, htab_pair++)
    {
        printf("%02ld", i);
        pair = pair_get_next(htab_pair);
        if (pair != NULL)
            print_pairs(pair);
        printf("\n");
    }
    printf("\n");
}

int htab_double_capacity(Htab htab)
{
    size_t new_capacity = htab_get_capacity(htab) * 2;

    Pair new_pair_list = calloc(new_capacity, sizeof(struct s_pair));
    if(new_pair_list == NULL)
    {
       return NO_SPACE;
    }

    for (size_t i = 0; i < htab->capacity; i++)
        for (Pair next = 0, pair = (htab->data + i)->next; pair != NULL; pair = next)
        {
            next = pair_get_next(pair);
            Buffer hkey = pair_get_hkey(pair);
            __pairlist_append_element(__pair_get_list_from_hkey(new_pair_list, hkey, new_capacity),
                                    pair);
        }

    free(htab->data);
    htab_set_capacity(htab, new_capacity);
    htab->data = new_pair_list;
    return SUCCESS;
}

int htab_insert(Htab htab, Buffer key, void *value)
{
    Buffer hkey;
    int error;
    Pair pair;

    error = construct_pair(&pair, key, value, &hkey);
    // increment size
    htab_set_size(htab, htab_get_size(htab) + 1);

    // If we need to double the capacity of our hash table
    if (htab_get_ratio(htab) > 75)
    {
        error = htab_double_capacity(htab);
        if(error != SUCCESS)
            return error;
    }

    // Get the position where the new hash should be
    Pair pair_list = __pair_get_list_from_hkey(htab_get_pair(htab),
                                               hkey,
                                               htab->capacity);

    // Add the new pair into the corresponding pair list
    __pairlist_append_element(pair_list, pair);
    return SUCCESS;
}