#ifndef SHARED_QUEUE
#define SHARED_QUEUE

#include <err.h>
#include <stdlib.h>
#include <semaphore.h>
#include "../builtins/queue/queue.h"

struct shared_queue
{
    sem_t lock;
    sem_t size;
    Queue queue;
};

typedef struct shared_queue shared_queue;

// Constructor
shared_queue *shared_queue_new();

// Function
void shared_queue_push(shared_queue *sq, int val);
int shared_queue_pop(shared_queue *sq);

// Destructor
void shared_queue_destroy(shared_queue *sq);

#endif
