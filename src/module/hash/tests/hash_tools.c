#include "hash_tools.h"

size_t max_size_t(size_t first, size_t second)
{
    if (first > second)
        return first;
    return second;
}

Buffer create_buff_hashed(Buffer *buff_result, string_t str, size_t size)
{
    Buffer buff;
    buffer_constructor_array(&buff, size, (u_char *)str);
    hash(buff_result, &buff);
    return buff;
}
