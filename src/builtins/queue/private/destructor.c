#include "../queue.h"

void _queue_proccess(Queue queue, Callback callback)
{
    // While the queue is not emtpy proccess
    while (!queue_is_empty(queue))
    {
        // Get an element
        void *element = queue_dequeue(queue);
        // If there is a callback function call it
        if (callback != NULL)
            callback(element);
    }
}

void _queue_foreach(Queue queue, Callback callback)
{
    // get the head
    struct _s_queue_block *block = queue->first;
    // while there is bock to proccess
    while (block != NULL)
    {
        // Proccess each element
        callback(block->element);
        // go to the next bock
        block = block->next;
    }
}
