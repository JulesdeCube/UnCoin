#include "../queue.h"

/**
** \private
**
*/
void queue_proccess(Queue queue, Callback callback);

/**
** \private
**
*/
void queue_foreach(Queue queue, Callback callback);

void queue_free(Queue queue, Destructor destructor)
{
    // Destroy all blocks of queue
    queue_proccess(queue, destructor);
    // Free the queue struct
    free(queue);
}
