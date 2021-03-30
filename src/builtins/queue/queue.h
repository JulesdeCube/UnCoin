#ifndef UNCOIN__BUILTINS_QUEUE__QUEUE_H_
#define UNCOIN__BUILTINS_QUEUE__QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "utils/bool.h"

typedef void (*Callback)(void *);
typedef Callback Destructor;

/**
** \private
**
** repesent
*/
struct _s_queue_block
{
    void *element;
    struct _s_queue_block *next;
    struct _s_queue_block *previous;
};

struct s_queue
{
    size_t size;
    struct _s_queue_block *first;
    struct _s_queue_block *last;
};

typedef struct s_queue *Queue;

Queue queue_init();
bool queue_is_empty(Queue queue);
void queue_enqueue(Queue queue, void *data);
void *queue_dequeue(Queue queue);

void queue_free(Queue queue, Destructor destructor);

void queue_debug(Queue queue);

#endif // UNCOIN__BUILTINS_QUEUE__QUEUE_H_
