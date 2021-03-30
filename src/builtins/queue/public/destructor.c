#include "../queue.h"

/**
** \private
**
*/
void _queue_proccess(Queue queue, Callback callback);

/**
** \private
**
*/
void _queue_foreach(Queue queue, Callback callback);

void queue_free(Queue queue, Destructor destructor)
{
    // Destroy all blocks of queue
    _queue_proccess(queue, destructor);
    // Free the queue struct
    free(queue);
}
