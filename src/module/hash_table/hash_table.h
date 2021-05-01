#ifndef UNCOIN__MODULE__HASH_TABLE__HASH_TABLE_H_
#define UNCOIN__MODULE__HASH_TABLE__HASH_TABLE_H_

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  LIBRARY                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "utils/error.h"
#include "utils/bool.h"
#include "builtins/buffer/buffer.h"
#include "../hash/hash.h"
#include "../hash/tests/hash_tools.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              TYPE DEFINTIONS                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

typedef void (*Callback)(void *);
typedef Callback Destructor;

/**
** \struct s_pair
** \brief a pair is something where we save some informations about a hashage
**
** \param hkey key after hashage
** \param key the key before the hash
** \param value The value associated with the key
** \param next the next s_pair
*/
struct s_pair
{
    Buffer hkey;
    Buffer key;
    void *value;
    struct s_pair *next;
};

typedef struct s_pair *Pair;

/**
** \struct s_htab
** \brief the Hash table
**
** \param capacity the length of the data array
** \param     size the number of actual elements within the data array
** \param     data each cell in this array will contain a linked list of
**                 'struct pair' values
*/
struct s_htab
{
    size_t capacity;
    size_t size;
    Pair data;
};

typedef struct s_htab *Htab;

// Default capacity of a hash table
#define DEFAULT_CAPACITY 4
// Ratio of the hash table equal to (capacity / size)
// result in percentage
#define MIN_RATIO 20
#define MAX_RATIO 80

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// TODO COMMENTS
int htab_constructor(Htab *new_htab);

int construct_pair(Pair *pair, Buffer key, void *value, Buffer *hkey);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// TODO comments
int htab_destructor(Htab htab, Destructor destructor);

int pair_destructor(Pair pair, Destructor destructor);

void list_clean(Pair list, Destructor destructor);

void htab_clear(Htab htab, Destructor destructor);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 OPERATOR                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int htab_get(Htab htab, Buffer key, void **value);
// TODO comments
void print_htab(Htab htab);

int htab_insert(Htab htab, Buffer key, void *value);

int construct_htab_from_array(Htab htab, size_t n_element, char **names,
                              void **values);

int htab_remove(Htab htab, Buffer key, Destructor destructor);

int htab_pop(Htab htab, Buffer key, void **value, Destructor destructor);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               GETTER/SETTER                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
** \brief get the size the number of actual elements within the data array
**
** \param htab the hash table
**
** \return the size the number of actual elements within the data array
**         or 0 if the htab is null
*/
size_t htab_get_size(Htab htab);

/**
** \brief set the size of the number of actual elements within the data array
**
** \param htab the hash table
** \param size the new size
*/
void htab_set_size(Htab htab, size_t size);

/**
** \brief get the length of the data array
**
** \param htab the hash table
**
** \return the length of the data array or 0 if the htab is null
*/
size_t htab_get_capacity(Htab htab);

/**
** \brief set the max capacity of elements can be in the data array
**
** \param htab the hash table
** \param new_capacity the new capacity
*/
void htab_set_capacity(Htab htab, size_t new_capacity);

/**
** \brief get pair
**
** \param htab the hash table
**
** \return the length of the data array or 0 if the htab is null
*/
Pair htab_get_pair(Htab htab);

/**
** \brief get ratio
**
** \param htab the hash table
**
** \return the ratio in percentage equal to (capacity / size)
*/
size_t htab_get_ratio(Htab htab);

// Comments TODO
Buffer pair_get_hkey(Pair pair);

Buffer pair_get_key(Pair pair);

void *pair_get_value(Pair pair);

Pair pair_get_next(Pair pair);

#endif // UNCOIN__MODULE__HASH_TABLE__HASH_TABLE_H_