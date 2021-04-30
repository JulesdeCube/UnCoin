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
Pair _get_pair_with_hashkey(Pair pair_list, Buffer hkey, Pair *previous_pair);

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

int __htab_reload_pairs(Htab htab, size_t new_capacity);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PUBLIC IMPLEMENTATIONS                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int htab_get_corresponding_pair(Htab htab,
                                Pair pair_to_test,
                                Pair *pair_result,
                                Pair *previous_pair_result)
{
    if (htab == NULL || pair_to_test == NULL)
        return NO_SELF;

    if (htab->size == 0)
        return OUT_OF_RANGE;

    // get the list of the possible position
    Pair where_can_be_list = __pair_get_list_from_hkey(htab->data,
                                                       pair_to_test->hkey,
                                                       htab->capacity);

    printf("test3.2\n");
    // Check if an element in the list has the same hash key
    Pair where_can_be_pair = _get_pair_with_hashkey(where_can_be_list,
                                                    pair_to_test->hkey,
                                                    previous_pair_result);
    printf("test3.3\n");
    // element not exist
    if (where_can_be_pair == where_can_be_list)
    {
        previous_pair_result = NULL;
        return OUT_OF_RANGE;
    }
    printf("test3.1\n");
    // test if it is not 2 different key with same hkey
    char *str1 = (char *)pair_to_test->key->data;
    char *str2 = (char *)where_can_be_pair->key->data;
    if (strcmp(str1, str2))
    {
        previous_pair_result = NULL;
        return OUT_OF_RANGE;
    }

    //we are good now
    *pair_result = where_can_be_pair;
    // if we found an element and the previous is null,
    // the previous have to be the sentinel
    if (previous_pair_result == NULL)
    {
        printf("test5\n");
        Pair *p = &where_can_be_list;
        printf("test5.2\n");
        previous_pair_result = p;
        printf("test5.3\n");
        if (previous_pair_result == NULL)
            printf("test5null\n");
    }
    return SUCCESS;
}

void print_pairs(Pair pair)
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
    return __htab_reload_pairs(htab, htab->capacity * 2);
}

int htab_divide_capacity(Htab htab)
{
    return __htab_reload_pairs(htab, htab->capacity / 2);
}

int htab_insert(Htab htab, Buffer key, void *value)
{
    Buffer hkey;
    int error;
    Pair pair;

    error = construct_pair(&pair, key, value, &hkey);
    if (error != SUCCESS)
        return error;
    // increment size
    htab_set_size(htab, htab_get_size(htab) + 1);

    // If we need to double the capacity of our hash table
    if (htab_get_ratio(htab) >= MAX_RATIO)
    {
        error = htab_double_capacity(htab);
        if (error != SUCCESS)
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

int construct_htab_from_array(Htab htab, size_t n_element, char **names,
                              void **values)
{
    if (htab == NULL)
        return NO_SELF;
    Buffer key;
    u_char *name;
    int error;
    for (size_t i = 0; i < n_element; ++i)
    {
        name = (u_char *)names[i];
        error = buffer_constructor_array(&key, strlen(names[i]) + 1, name);
        if (error != SUCCESS)
            return error;
        error = htab_insert(htab, key, values[i]);
        if (error != SUCCESS)
            return error;
    }
    return SUCCESS;
}

int htab_remove_pair(Htab htab, Pair pair_to_remove, Destructor destructor)
{
    if (htab == NULL || pair_to_remove == NULL)
        return NO_SELF;

    //The pair in the htab for remove it
    Pair corresponding_pair;
    Pair previous_pair;

    int error = htab_get_corresponding_pair(htab,
                                            pair_to_remove,
                                            &corresponding_pair,
                                            &previous_pair);
    if (error != SUCCESS)
        return error;

    printf("test1\n");
    // remove in the hash table
    previous_pair->next = corresponding_pair->next;
    printf("test A AVOIR ABSOLUMENT\n");

    // change capacity oh the hash table if we need
    htab->size -= 1;
    if (htab_get_ratio(htab) <= MIN_RATIO)
    {
        error = htab_divide_capacity(htab);
        if (error != SUCCESS)
            return error;
    }

    // destruct the pair (free all things)
    return pair_destructor(corresponding_pair, destructor);
}