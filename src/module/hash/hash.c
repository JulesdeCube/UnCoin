#include "hash.h"

int hash_from_buffer(size_t *hash, Buffer hash_buf)
{
    // Use the Jenkins's one_at_a_time hash.
    if (hash_buf == NULL)
        return NO_SELF;
    if (hash == NULL)
        return ERROR_VALUE;
    u_char *key = buffer_get_data(hash_buf);
    if (key == NULL)
        return INTERNAL_ERROR;
    size_t size = buffer_get_size(hash_buf);

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

int hash(Buffer *buff_result, Buffer buff_to_hash)
{
    size_t hash;
    size_t *p_hash = &hash;
    // call hash function
    int error = hash_from_buffer(p_hash, buff_to_hash);
    if (error != SUCCESS)
        return error;

    // change type and call the function to construct.
    void *array = p_hash;
    return buffer_constructor_array(buff_result, sizeof(size_t), array);
}