#define _GNU_SOURCE

#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "shared_queue.h"

// Number of threads.
const size_t THREAD_COUNT = 3;

int rerecv(int fd, char **str, size_t *count)
{
    char buffer = '\0';
    size_t len = 1;
    int r = 0;

    if (*str == NULL)
    {
        *str = calloc(1, sizeof(char));
        if (*str == NULL)
            errx(EXIT_FAILURE, "Can't allocate memory in worker()");
    }

    while ((r = recv(fd, &buffer, 1, 0)) != 0)
    {
        if (r == -1)
            return 0;

        if (buffer == '\n')
            return 1;

        *str = realloc(*str, (len + 1) * sizeof(char));
        (*str)[len - 1] = buffer;
        (*str)[len] = '\0';
        len += 1;
    }

    *count = len;
    return 1;
}
// Function executed by the threads.
void *worker(void *arg)
{
    // Gets the shared queue.
    shared_queue *queue = arg;

    while (1)
    {
        int fd = shared_queue_pop(queue);

        char *str = NULL;
        size_t len = 0;
        int r = 0;

        // Read transaction from client
        r = rerecv(fd, &str, &len);

        // Send response to client
        if (r > 0)
        {
            printf("The server received: \"%s\"\n", str);
            r = send(fd, "OK", 2, 0);
            //if (r > 0)
            //    printf("Message OK sent to client\n");
            //else
            //    printf("(OK)Message fail sent to client\n");
        }
        else
        {
            printf("The server received nothing (error)\n");
            r = send(fd, "KO", 2, 0);
            //if (r > 0)
            //    printf("Message KO sent to client\n");
            //else
            //    printf("(KO)Message fail send to client\n");
        }

        free(str);
        close(fd);
    }
    return NULL;
}

int main()
{
    // Creates the shared queue.
    shared_queue *queue = shared_queue_new();

    pthread_t data[THREAD_COUNT];
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        int e = pthread_create(data + i, NULL, worker, queue);
        if (e != 0)
            errx(EXIT_FAILURE, "Could not join the thread %zu.", i);
    }

    struct addrinfo hints;
    struct addrinfo *result;
    int addrinfo_error;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       //IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     //Server Mode

    addrinfo_error = getaddrinfo(NULL, "2048", &hints, &result);
    if (addrinfo_error == -1)
    {
        errx(EXIT_FAILURE, "Fail getting address on port: %s",
             gai_strerror(addrinfo_error));
    }

    int cnx = 0;
    struct addrinfo *rp;
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        cnx = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cnx == -1)
            continue;

        int value = 1;
        if (setsockopt(cnx, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) == -1)
            errx(EXIT_FAILURE, "Error setsockopt()");

        if (bind(cnx, rp->ai_addr, sizeof(struct sockaddr)) == 0)
            break;

        close(cnx);
    }

    if (rp == NULL)
        errx(EXIT_FAILURE, "Error: no socket has been open");

    if (listen(cnx, 5) == -1)
        errx(EXIT_FAILURE, "Error listen()");

    int fdcnx = 0;
    while (1)
    {
        // Waiting
        fdcnx = accept(cnx, rp->ai_addr, &rp->ai_addrlen);
        if (fdcnx == -1)
            errx(EXIT_FAILURE, "Error accept()");

        shared_queue_push(queue, fdcnx);
    }

    // End
    freeaddrinfo(result);
    shared_queue_destroy(queue);
    close(fdcnx);
    close(cnx);

    return 0;
}