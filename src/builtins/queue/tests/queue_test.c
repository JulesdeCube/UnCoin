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
    // Variable command
    int cmd = -1;

    // Queue
    Queue q = NULL;

    // Start or Quit
    printf("Start (0) or Quit (4)? : ");
    scanf("%d", &cmd);
    printf("\n");

    // QUIT
    if (cmd == 4)
        return 0;

    // Init
    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||                    START                     ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");
    q = queue_init();

    // Command help
    printf("--------------------------------------------------\n");
    printf(">                    Command                     <\n");
    printf(">                                                <\n");
    printf(">    1 - Enqueue                                 <\n");
    printf(">    2 - Dequeue                                 <\n");
    printf(">    3 - Print queue                             <\n");
    printf(">    4 - Quit                                    <\n");
    printf(">                                                <\n");
    printf("--------------------------------------------------\n");

    // WHILE NOT QUIT
    while (cmd != 4)
    {
        // COMMAND
        printf("\n(cmd) : ");
        while (1)
        {
            getchar();
            if (scanf("%d", &cmd) != 1)
                printf("Invalid input. Try again\n");
            else if (cmd < 1 || cmd > 4)
                printf("Invalid input. Try again\n");
            else
                break;
        }

        // ENQUEUE
        if (cmd == 1)
        {
            printf("(Enqueue) Enter element : ");
            char *element = NULL;
            while(1)
            {
                getchar();
                if (scanf("%m[a-zA-Z0-9]", &element) != 1)
                    printf("Invalid input. Try again\n");
                else
                    break;
            }
            queue_enqueue(q, element);
        }

        // DEQUEUE
        else if (cmd == 2)
            printf("(Dequeue) %s\n", (char *)queue_dequeue(q));

        // PRINT QUEUE
        else if (cmd == 3)
            queue_debug(q);
    }

    // Free
    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||                     END                      ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");
    queue_free(q, NULL);

    return 0;
}
