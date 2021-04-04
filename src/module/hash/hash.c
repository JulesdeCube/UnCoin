#include "hash.h"
#include <math.h>

int hash_from_buffer(size_t *hash, Buffer *hash_buf)
{
    // Use the Jenkins's one_at_a_time hash.
    u_char *key = buffer_get_data(*hash_buf);
    size_t h = 0;
    while (*key != '\0')
    {
        h += *key;
        h += h << 10;
        h ^= h >> 6;
        ++key;
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
    hash_from_buffer(p_hash, buf_to_hash);

    void *array = p_hash;
    int error = buffer_constructor_array(hash_result, sizeof(size_t), array);
    return error;
}