#include <stdio.h>
#include <stdlib.h>
#include <err.h>

struct queue
{
    size_t size;
    struct elm *first;
    struct elm *last;
};

struct elm
{
    void *data;
    struct elm *next;
    struct elm *previous;
};

struct queue* queue_init()
{
    // Allocate for a new queue
    struct queue *q = malloc(sizeof(struct queue));

    if (q == NULL)
        errx(EXIT_FAILURE, "Error: malloc in queue_init()");

    // Initialize first and last elm of q
    q->first = NULL;
    q->last = NULL;

    return q;
}

void queue_enqueue(struct queue *q, void *data)
{
    // Allocate for a new element
    struct elm *element = malloc(sizeof(struct elm));

    if (element == NULL)
        errx(EXIT_FAILURE, "Error: malloc in queue_enqueue()");

    // Initialize data and next of the new elm
    element->data = data;
    element->next = NULL;

    // Initialize the first elm of q if it is empty
    // Initialize previous of elm to NULL (no elm before the new elm)
    if (q->first == NULL)
    {
        element->previous = NULL;
        q->first = element;
    }

    // Initialize previous of elm to the last elm in q
    // (have elm before the new elm)
    // Reinitialize next of the last elm to the new elm
    else
    {
        element->previous = q->last;
        q->last->next = element;
    }

    // Reinitialize the last elm of q
    q->last = element;

    // Incremente the size for the new elm
    q->size += 1;
}

struct elm* queue_dequeue(struct queue *q)
{
    // q is empty
    // Do nothing, return NULL
    if (q->size == 0)
        return NULL;

    // Pop the first elm of q
    struct elm *first_elm = q->first;

    // Reinitialize the first elm of q to the next
    q->first = q->first->next;

    // q is empty after pop/dequeue
    if (q->first == NULL)
        q->last = NULL;

    // q is not empty after pop/dequeue
    // Reinitialize the previous of the new first elm
    else
        q->first->previous = NULL;

    // Decremente the size of q
    q->size--;

    // Reinitialize the elm
    first_elm->next = NULL;

    return first_elm;
}

void queue_free(struct queue *q)
{
    // Free all elm of q (while q is not empty)
    while (q->size > 0)
        free(queue_dequeue(q));

    // Free the queue
    free(q);
}

void queue_print_str(struct queue *q)
{
    // Print queue all data (char *)
    for (struct elm *tmp = q->first; tmp != NULL; tmp = tmp->next)
        printf("%p : \"%s\"\n", tmp->data, (char*)tmp->data);

    printf("\n");
}

void queue_print_int(struct queue *q)
{
    // Print queue all data (size_t)
    for (struct elm *tmp = q->first; tmp != NULL; tmp = tmp->next)
        printf("%p : %ld\n", tmp->data, (size_t)tmp->data);
    
    printf("\n");
}

int main()
{
    // Test init
    struct queue *q = queue_init();
    struct queue *q1 = queue_init();
    struct queue *q2 = queue_init();


    // Test enqueue of q
    printf("[Enqueue q \"Hello\"]\n");
    queue_enqueue(q, "Hello");
    queue_print_str(q);

    printf("[Enqueue q \"1\"]\n");
    queue_enqueue(q, "1");
    queue_print_str(q);

    printf("[Enqueue q \"400000\"]\n");
    queue_enqueue(q, "400000");
    queue_print_str(q);

    // Test enqueue of q1
    printf("[Enqueue q1 \"Helloooooooo\"]\n");
    queue_enqueue(q1, "Helloooooooo");
    queue_print_str(q1);

    // Test enqueue of q2
    printf("[Enqueue q2 400000]\n");
    queue_enqueue(q2, (void *)400000);
    queue_print_int(q2);


    // Test dequeue
    printf("[Dequeue q1 \"Helloooooooo\"]\n");
    struct elm *elm = queue_dequeue(q1);
    printf("Elm dequeue : \"%s\"\n", (char*)elm->data);
    queue_print_str(q1);

    // Print all queue
    printf("[Print queue q char* no empty]\n");
    queue_print_str(q);

    printf("[Print queue q1 char* empty]\n");
    queue_print_str(q1);

    printf("[Print queue q2 size_t no empty]\n");
    queue_print_int(q2);

    // Free
    free(elm);
    queue_free(q);
    queue_free(q1);
    queue_free(q2);

    return 0;
}
