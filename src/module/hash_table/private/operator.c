#include "../hash_table.h"

int _buffer_equal(Buffer buffer1, Buffer buffer2, bool *result)
{
    if (result == NULL)
        return ERROR_VALUE;

    if (buffer1 == NULL || buffer2 == NULL)
    {
        *result = false;
        return NO_SELF;
    }

    size_t size1 = buffer_get_size(buffer1);
    size_t size2 = buffer_get_size(buffer2);
    *result = size1 == size2;

    if (!*result)
        return SUCCESS;

    u_char *data1 = buffer_get_data(buffer1);
    u_char *data2 = buffer_get_data(buffer2);

    for (; size1 && *data1++ == *data2++; --size1)
        ;

    *result = size1 == 0;
    return SUCCESS;
}

_Pair _get_pair_with_hashkey(_Pair pair_list, Buffer hkey, _Pair *previous_pair)
{
    *previous_pair = pair_list;
    _Pair pair = pair_list->next;
    bool result;
    for (; pair != NULL; *previous_pair = pair, pair = pair->next)
    {
        _buffer_equal(pair->hkey, hkey, &result);
        if (result)
            return pair;
    }
    return NULL;
}

_Pair _pair_get_list_from_hkey(_Pair pair, Buffer hkey, size_t capacity)
{
    size_t *wait = (size_t *)buffer_get_data(hkey);
    size_t res = *wait % capacity;
    return pair + res;
}

void _pairlist_append_element(_Pair pair_list, _Pair element)
{
    element->next = pair_list->next;
    pair_list->next = element;
}

int _htab_reload_pairs(Htab htab, size_t new_capacity)
{
    _Pair new_pair_list = calloc(new_capacity, sizeof(struct s_pair));

    if (new_pair_list == NULL)
        return NO_SPACE;

    for (size_t i = 0; i < htab->capacity; i++)
        for (_Pair next = 0, pair = (htab->data + i)->next; pair != NULL; pair = next)
        {
            next = pair->next;
            Buffer hkey = pair->hkey;
            _pairlist_append_element(
                _pair_get_list_from_hkey(new_pair_list,
                                         hkey,
                                         new_capacity),
                pair);
        }

    free(htab->data);
    htab->capacity = new_capacity;
    htab->data = new_pair_list;

    return SUCCESS;
}

int _htab_get_corresponding_pair(Htab htab,
                                 Buffer key,
                                 _Pair *pair_result,
                                 _Pair *previous_pair)
{
    if (htab == NULL || key == NULL)
        return NO_SELF;

    if (htab->size == 0)
        return OUT_OF_RANGE;

    Buffer hkey;
    int error = hash(&hkey, &key);
    if (error != SUCCESS)
        return error;

    // get the list of the possible position
    _Pair where_can_be_list = _pair_get_list_from_hkey(htab->data,
                                                       hkey,
                                                       htab->capacity);
    bool result;
    _Pair where_can_be_pair;
    do
    {
        // Check if an element in the list has the same hash key
        where_can_be_pair = _get_pair_with_hashkey(where_can_be_list,
                                                        hkey,
                                                        previous_pair);
        if(where_can_be_pair == NULL)
            return OUT_OF_RANGE;

        // test if it is not 2 different key with same hkey
        error = _buffer_equal(key, where_can_be_pair->key, &result);
        if (error != SUCCESS)
            return error;
        where_can_be_list = where_can_be_pair;
    }
    while(!result);
    *pair_result = where_can_be_pair;
    return SUCCESS;

}

void _print_pairs(_Pair pair)
{
    char *s_hkey, *s_key;

    while (pair != NULL)
    {
        buffer_to_hex(pair->hkey, &s_hkey, NULL);
        buffer_to_hex(pair->key, &s_key, NULL);

        printf(" -> (%s, %s, %p)", s_hkey, s_key, pair->value);

        free(s_hkey);
        free(s_key);
        // take the next pair.
        pair = pair->next;
    }
}

int _htab_double_capacity(Htab htab)
{
    return _htab_reload_pairs(htab, htab->capacity * 2);
}


int _htab_divide_capacity(Htab htab)
{
    if (htab->capacity == DEFAULT_CAPACITY)
        return SUCCESS;
    return _htab_reload_pairs(htab, htab->capacity / 2);
}