#include "../queue.h"

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
    {
        // Print block infos
        printf(
            "%p:%p (%p <> %p)\n",
            block,
            block->element,
            block->previous,
            block->next);
        block = block->next;
    }
    // End of the queue
    printf("===========================================\n\n");
}

int main()
{
    // Test init
    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||                     INIT                     ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");

    printf("[Init q]\n");
    Queue q = queue_init();
    queue_debug(q);

    printf("[Init q1]\n");
    Queue q1 = queue_init();
    queue_debug(q1);

    printf("[Init q2]\n");
    Queue q2 = queue_init();
    queue_debug(q2);

    // Test enqueue
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
    queue_debug(q1);

    // Test enqueue of q2
    printf("[Enqueue q2 400000]\n");
    queue_enqueue(q2, (void *)400000);
    queue_debug(q);

    // Test dequeue
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

    // Test free
    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||                     FREE                     ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");

    printf("[free q]\n");
    queue_free(q, NULL);

    printf("[free q1]\n");
    queue_free(q1, NULL);

    printf("[free q2]\n");
    queue_free(q2, NULL);

    return 0;
}
