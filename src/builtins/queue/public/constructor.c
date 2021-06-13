#include "../queue.h"

Queue queue_init()
{
    // Allocate for a new queue
    Queue queue = malloc(sizeof(struct s_queue));

    if (queue == NULL)
        errx(EXIT_FAILURE, "Error: malloc in queue_init()");

    // Initialize first and last element of queue
    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;

    return queue;
}

bool_t queue_is_empty(Queue queue)
{
    return queue == NULL || queue->size <= 0;
}

void queue_enqueue(Queue queue, void *data)
{
    // Allocate for a new block
    struct _s_queue_block *block = malloc(sizeof(struct _s_queue_block));

    if (block == NULL)
        errx(EXIT_FAILURE, "Error: malloc in queue_enqueue()");

    // Initialize data and next of the new element
    block->element = data;
    block->next = NULL;

    if (queue_is_empty(queue))
        // Initialize the first elm of q if it is empty
        queue->first = block;
    else
        // Initialize previous of elm to the last elm in q
        queue->last->next = block;

    // Set the previous to be the last (NULL if queue is empty)
    block->previous = queue->last;
    // Reinitialize the last elm of q
    queue->last = block;

    // Incremente the size for the new elm
    queue->size++;
}

void *queue_dequeue(Queue queue)
{
    // queue is empty
    // Do nothing, return NULL
    if (queue_is_empty(queue))
        return NULL;

    // Pop the first block of the queue
    struct _s_queue_block *head = queue->first;

    // Relink the fist element of the queue
    queue->first = queue->first->next;

    // if queue is empty after pop/dequeue
    if (queue->first == NULL)
        // reinit the queue
        queue->last = NULL;

    // if queue is not empty after pop/dequeue
    else
        // Reinitialize the previous of the new first elm
        queue->first->previous = NULL;

    // Decremente the size
    queue->size--;

    // Get the element
    void *element = head->element;

    // free the poped block
    free(head);

    return element;
}
