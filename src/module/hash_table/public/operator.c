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
_Pair _get_pair_with_hashkey(_Pair pair_list, Buffer hkey, _Pair *previous_pair);

/**
** \private
**
** \brief
*/
_Pair _pair_get_list_from_hkey(_Pair pair, Buffer hkey, size_t capacity);

/**
** \private
**
** \brief
*/
void _pairlist_append_element(_Pair pair_list, _Pair element);

int _htab_reload_pairs(Htab htab, size_t new_capacity);

int _buffer_equal(Buffer buffer1, Buffer buffer2, bool *result);

int _htab_get_corresponding_pair(Htab htab,
                                 Buffer key,
                                 _Pair *pair_result,
                                 _Pair *previous_pair);

void _htab_pair_destructor(_Pair pair, Destructor destructor);

int _htab_constructor_pair(_Pair *pair, Buffer key, void *value, Buffer *hkey);

void _print_pairs(_Pair pair);

int _htab_double_capacity(Htab htab);

int _htab_divide_capacity(Htab htab);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             PUBLIC IMPLEMENTATIONS                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int htab_get(Htab htab, Buffer key, void **value)
{
    _Pair pair_result, previous_pair;
    int error = _htab_get_corresponding_pair(htab, key, &pair_result, &previous_pair);
    if (error != SUCCESS)
        return error;

    *value = pair_result->value;
    return SUCCESS;
}

void htab_print(Htab htab)
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

    _Pair pair;
    // Get the first pair
    _Pair pairs = htab->data;
    // run and print on all pairs
    for (size_t i = 0; i < capacity; ++i, pairs++)
    {
        printf("%02ld", i);
        pair = pairs->next;

        if (pair != NULL)
            _print_pairs(pair);

        printf("\n");
    }
    printf("\n");
}

int htab_insert(Htab htab, Buffer key, void *value)
{
    Buffer hkey;
    int error;
    _Pair pair;

    _htab_constructor_pair(&pair, key, value, &hkey);

    // increment size
    htab->size++;

    // If we need to double the capacity of our hash table
    if (htab_get_ratio(htab) >= MAX_RATIO)
    {
        error = _htab_double_capacity(htab);
        if (error != SUCCESS)
            return error;
    }

    // Get the position where the new hash should be
    _Pair pair_list = _pair_get_list_from_hkey(htab->data,
                                               hkey,
                                               htab->capacity);

    // Add the new pair into the corresponding pair list
    _pairlist_append_element(pair_list, pair);
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

int htab_remove(Htab htab, Buffer key, Destructor destructor)
{
    _Pair corresponding_pair, previous_pair;
    int error = _htab_get_corresponding_pair(htab,
                                             key,
                                             &corresponding_pair,
                                             &previous_pair);
    if (error != SUCCESS)
        return error;

    // remove in the hash table
    previous_pair->next = corresponding_pair->next;

    // change capacity oh the hash table if we need
    htab->size -= 1;
    if (htab_get_ratio(htab) <= MIN_RATIO)
    {
        error = _htab_divide_capacity(htab);
        if (error != SUCCESS)
            return error;
    }

    // destruct the pair (free all things)
    _htab_pair_destructor(corresponding_pair, destructor);

    return SUCCESS;
}

int htab_pop(Htab htab, Buffer key, void **value, Destructor destructor)
{
    _Pair corresponding_pair, previous_pair;
    int error = _htab_get_corresponding_pair(htab,
                                             key,
                                             &corresponding_pair,
                                             &previous_pair);
    if (error != SUCCESS)
        return error;

    *value = corresponding_pair->value;

    // remove in the hash table
    previous_pair->next = corresponding_pair->next;

    // change capacity oh the hash table if we need
    htab->size -= 1;
    if (htab_get_ratio(htab) <= MIN_RATIO)
    {
        error = _htab_divide_capacity(htab);
        if (error != SUCCESS)
            return error;
    }
    // destruct the pair (free all things)
    _htab_pair_destructor(corresponding_pair, destructor);

    return SUCCESS;
}