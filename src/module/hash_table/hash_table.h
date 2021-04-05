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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              TYPE DEFINTIONS                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

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


#define DEFAULT_CAPACITY 4

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                CONSTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// TODO COMMENTS
int htab_constructor(Htab *new_htab);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 DESTRUCTOR                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// TODO comments
int htab_destructor(Htab htab);

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
** \brief get pair
**
** \param htab the hash table
**
** \return the length of the data array or 0 if the htab is null
*/
Pair htab_get_pair(Htab htab);

// Comments TODO
Buffer pair_get_hkey(Pair pair);

Buffer pair_get_key(Pair pair);

void *pair_get_value(Pair pair);

Pair pair_get_next(Pair pair);

#endif // UNCOIN__MODULE__HASH_TABLE__HASH_TABLE_H_