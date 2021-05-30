#include "shared_queue.h"

shared_queue *shared_queue_new()
{
    shared_queue *s_queue = malloc(sizeof(shared_queue));
    if (s_queue == NULL)
        errx(EXIT_FAILURE, "malloc shared_queue_new()");

    sem_t sem;
    if (sem_init(&sem, 0, 1) != 0)
        errx(EXIT_FAILURE, "init semaphore in shared_queue_new()");

    sem_t size;
    if (sem_init(&size, 0, 0) != 0)
        errx(EXIT_FAILURE, "init semaphore in shared_queue_new()");

    s_queue->lock = sem;
    s_queue->size = size;
    s_queue->queue = queue_init();

    return s_queue;
}

void shared_queue_push(shared_queue *sq, int val)
{
    // Lock semaphore
    sem_wait(&(sq->lock));
    queue_enqueue(sq->queue, &val);
    sem_post(&(sq->lock));

    // Size semaphore (+)
    sem_post(&(sq->size));
}

int shared_queue_pop(shared_queue *sq)
{
    // Size semaphore (-)
    sem_wait(&(sq->size));

    // Lock semaphore
    sem_wait(&(sq->lock));
    int val = 0;
    val = *((int *)queue_dequeue(sq->queue));
    sem_post(&(sq->lock));

    return val;
}

void shared_queue_destroy(shared_queue *sq)
{
    while (sq->queue != NULL)
        shared_queue_pop(sq);

    sem_destroy(&(sq->lock));
    sem_destroy(&(sq->size));

    queue_free(sq->queue, NULL);
    free(sq->queue);
    free(sq);
}
