#include "hash_tools.h"

size_t max_size_t(size_t first, size_t second)
{
    if (first > second)
        return first;
    return second;
}

Buffer *create_buff_hashed(Buffer *buff_result ,char *str)
{
    Buffer buff;
    buffer_constructor_str(&buff, str, true);
    hash(buff_result, &buff);
    Buffer *p_buff = &buff;
    return p_buff;
}