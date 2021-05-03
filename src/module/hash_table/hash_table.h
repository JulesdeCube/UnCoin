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

typedef struct s_pair *_Pair;

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
    _Pair data;
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

int construct_htab_from_array(Htab htab, size_t n_element, char **names,
                              void **values);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// TODO comments
void htab_destructor(Htab htab, Destructor destructor);

void htab_clear(Htab htab, Destructor destructor);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 OPERATOR                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// TODO comments

int htab_get(Htab htab, Buffer key, void **value);

int htab_insert(Htab htab, Buffer key, void *value);

int htab_remove(Htab htab, Buffer key, Destructor destructor);

int htab_pop(Htab htab, Buffer key, void **value, Destructor destructor);

void htab_print(Htab htab);

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
** \brief get the length of the data array
**
** \param htab the hash table
**
** \return the length of the data array or 0 if the htab is null
*/
size_t htab_get_capacity(Htab htab);

/**
** \brief get ratio
**
** \param htab the hash table
**
** \return the ratio in percentage equal to (capacity / size)
*/
size_t htab_get_ratio(Htab htab);

#endif // UNCOIN__MODULE__HASH_TABLE__HASH_TABLE_H_