#include "hash.h"
#include <math.h>

int hash_from_buffer(size_t *hash, Buffer *hash_buf)
{
    // Use the Jenkins's one_at_a_time hash.
    u_char *key = buffer_get_data(*hash_buf);
    size_t size = buffer_get_size(*hash_buf);
    if(size == 0)
        return NO_SELF;
    size_t h = 0;
    for (size_t i = 0; i < size; ++i, ++key)
    {
        h += *key;
        h += h << 10;
        h ^= h >> 6;
    }
    h += h << 3;
    h ^= h >> 11;
    h += h << 15;

    *hash = h;
    return SUCCESS;
}

int hash(Buffer *hash_result, Buffer *buf_to_hash)
{
    size_t hash;
    size_t *p_hash = &hash;
    // call hash function
    hash_from_buffer(p_hash, buf_to_hash);

    // change type and call the function to construct.
    void *array = p_hash;
    return buffer_constructor_array(hash_result, sizeof(size_t), array);
}