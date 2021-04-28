#include "../hash_table.h"

Pair _get_pair_with_hashkey(Pair pair, Buffer hkey)
{
    if (pair == NULL)
        return NULL;
    char *str1 = (char *) pair->hkey->data;
    char *str2 = (char *) hkey->data;
    if(!strcmp(str1, str2))
        return pair;
    return _get_pair_with_hashkey(pair->next, hkey);
}

Pair __pair_get_list_from_hkey(Pair pair, Buffer hkey, size_t capacity)
{
    size_t *wait = (size_t *) buffer_get_data(hkey);
    size_t res = *wait % capacity;
    return pair + res;
}

void __pairlist_append_element(Pair pair_list, Pair element)
{
    element->next = pair_list->next;
    pair_list->next = element;
}