#include <stdio.h>
#include <stdlib.h>
#include <err.h>

typedef int bool;
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

bool queue_is_empty(Queue queue)
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

void queue_proccess(Queue queue, Callback callback)
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

void queue_foreach(Queue queue, Callback callback)
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

void queue_debug(Queue queue)
{
    // Print queue info
    printf("===========================================\n");
    printf("Queue: %lu (%p <> %p)\n", queue->size, queue->first, queue->last);
    printf("-------------------------------------------\n");

    // get the head
    struct _s_queue_block *block = queue->first;
    // while there is bock to proccess
    while (block != NULL)
        // Print block infos
        printf(
            "%p:%p (%p <> %p)\n",
            block,
            block->element,
            block->previous,
            block->next);
        block = block->next;

    // End of the queue
    printf("===========================================\n\n");
}

void queue_free(Queue queue, Destructor destructor)
{
    // Destroy all blocks of queue
    queue_proccess(queue, destructor);
    // Free the queue struct
    free(queue);
}

int main()
{
    // Test init
    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||                     INIT                     ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");

    printf("[init q]\n");
    Queue q = queue_init();
    queue_debug(q);

    printf("[init q1]\n");
    Queue q1 = queue_init();
    queue_debug(q1);

    printf("[init q2]\n");
    Queue q2 = queue_init();
    queue_debug(q2);

    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||                   ENQUEUE                    ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");
    // Test enqueue of q
    printf("[Enqueue q \"Hello\"]\n");
    queue_enqueue(q, "Hello");
    queue_debug(q);

    printf("[Enqueue q \"1\"]\n");
    queue_enqueue(q, "1");
    queue_debug(q);

    printf("[Enqueue q \"400000\"]\n");
    queue_enqueue(q, "400000");
    queue_debug(q);

    // Test enqueue of q1
    printf("[Enqueue q1 \"Helloooooooo\"]\n");
    queue_enqueue(q1, "Helloooooooo");
    queue_debug(q);

    // Test enqueue of q2
    printf("[Enqueue q2 400000]\n");
    queue_enqueue(q2, (void *)400000);
    queue_debug(q);

    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||                   DEQUEUE                    ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");

    printf("[Dequeue q \"hello\"]\n");
    printf("Elm dequeue : \"%s\"\n", (char *)queue_dequeue(q));
    queue_debug(q);

    printf("[Dequeue q \"1\"]\n");
    printf("Elm dequeue : \"%s\"\n", (char *)queue_dequeue(q));
    queue_debug(q);

    printf("[Dequeue q1 \"Helloooooooo\"]\n");
    printf("Elm dequeue : \"%s\"\n", (char *)queue_dequeue(q1));
    queue_debug(q1);

    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||                     FREE                     ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");
    // Free
    printf("[free q]\n");
    queue_free(q, NULL);
    printf("[free q1]\n");
    queue_free(q1, NULL);
    printf("[free q2]\n");
    queue_free(q2, NULL);

    return 0;
}
