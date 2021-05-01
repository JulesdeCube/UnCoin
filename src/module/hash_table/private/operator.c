#include "../hash_table.h"

int __buffer_equal(Buffer buffer1, Buffer buffer2, bool *result)
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

Pair _get_pair_with_hashkey(Pair pair_list, Buffer hkey, Pair *previous_pair)
{
    *previous_pair = pair_list;
    Pair pair = pair_list->next;
    for (; pair != NULL; *previous_pair = pair, pair = pair->next)
    {
        bool result;
        __buffer_equal(pair->hkey, hkey, &result);
        if (result)
            return pair;
    }
    return NULL;
}

Pair __pair_get_list_from_hkey(Pair pair, Buffer hkey, size_t capacity)
{
    size_t *wait = (size_t *)buffer_get_data(hkey);
    size_t res = *wait % capacity;
    return pair + res;
}

void __pairlist_append_element(Pair pair_list, Pair element)
{
    element->next = pair_list->next;
    pair_list->next = element;
}

int __htab_reload_pairs(Htab htab, size_t new_capacity)
{
    Pair new_pair_list = calloc(new_capacity, sizeof(struct s_pair));
    if (new_pair_list == NULL)
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

int __htab_get_corresponding_pair(Htab htab,
                                Buffer key,
                                Pair *pair_result,
                                Pair *previous_pair)
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
    Pair where_can_be_list = __pair_get_list_from_hkey(htab->data,
                                                       hkey,
                                                       htab->capacity);
    // Check if an element in the list has the same hash key
    Pair where_can_be_pair = _get_pair_with_hashkey(where_can_be_list,
                                                    hkey,
                                                    previous_pair);
    // element not exist
    if (where_can_be_pair == NULL)
    {
        return OUT_OF_RANGE;
    }

    // test if it is not 2 different key with same hkey
    bool result;
    error = __buffer_equal(key, where_can_be_pair->key, &result);
    if (error != SUCCESS)
        return error;
    if (!result)
    {
        return OUT_OF_RANGE;
    }
    *pair_result = where_can_be_pair;
    return SUCCESS;
}