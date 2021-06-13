#include "../network.h"

void _network_clean_read(int fd, char *cmd)
{
    int r = 0;
    while (*cmd != '\n')
    {
        r = read(fd, cmd, 1);
        if (r == -1)
            errx(EXIT_FAILURE, "Error read");
    }
}

void _network_read(int fd, Buffer *b)
{
    char buffer = 0;
    int r = 0;

    buffer_constructor_size(b, 1);
    r = read(fd, &buffer, 1);
    if (r == -1)
        errx(EXIT_FAILURE, "Error read");

    while (buffer != '\n')
    {
        (*b)->data = (u_char *)realloc((char *)buffer_get_data(*b), buffer_get_size(*b) + 1);
        strncat((char *)buffer_get_data(*b), &buffer, 1);
        r = read(fd, &buffer, 1);
        if (r == -1)
            errx(EXIT_FAILURE, "Error read");
    }
}

void _network_read_infoTransaction(int fdcnx, Buffer *b_from, Buffer *b_to, Buffer *b_privKey, Buffer *b_amount)
{
    write(fdcnx, "(Transaction) from : ", strlen("(Transaction) from : "));
    _network_read(fdcnx, b_from);

    write(fdcnx, "(Transaction) to : ", strlen("(Transaction) to : "));
    _network_read(fdcnx, b_to);

    write(fdcnx, "(Transaction) privKey : ", strlen("(Transaction) privKey : "));
    _network_read(fdcnx, b_privKey);

    write(fdcnx, "(Transaction) amount : ", strlen("(Transaction) amount : "));
    _network_read(fdcnx, b_amount);
}

/*void _network_transaction(Blockchain bc, char *from, char *to, long amount, char *privKey)
{
    // Verification
    // TODO

    // Transaction
    Transaction p_tran;
    Buffer b_from, b_to, b_privKey;

    // Create buffer for str
    buffer_constructor_str(&b_from, from, true);
    buffer_constructor_str(&b_to, to, true);
    buffer_constructor_str(&b_privKey, b_privKey, true);
    
    transaction_constructor(&p_tran, b_from, b_to, amount, b_privKey);

    // Blockchain
    blockchain_block_add(bc, p_tran);
}*/

void* _network_thread(void *arg)
{
    int fdcnx = *(int *)arg;
    printf("Hello fdcnx(%i) !\n", fdcnx);

    while(1)
    {
        // In fdcnx terminal
        write(fdcnx, "(cmd) : ", 8);

        char cmd;
        int r = 0;

        r = read(fdcnx, &cmd, 1);
        if (r <= 0)
            break;

        // Command
        switch (cmd)
        {
            case 't': // Transaction On!
                // Cleat reading
                _network_clean_read(fdcnx, &cmd);

                // Read info for transaction
                Buffer b_from, b_to, b_privKey, b_amount;
                _network_read_infoTransaction(fdcnx, &b_from, &b_to, &b_privKey, &b_amount);

                // Debug
                printf("Transaction (%i) : { From: %s, To: %s, PrivKey: %s, Amount: %s }\n",
                         fdcnx, (char *)b_from->data, (char *)b_to->data,
                                (char *)b_privKey->data, (char *)b_amount->data);
                
                // TODO
                // strtol() char* to long
                
                // Free buffer (TODO)
                buffer_destructor_safe(&b_from);
                buffer_destructor_safe(&b_to);
                buffer_destructor_safe(&b_privKey);
                buffer_destructor_safe(&b_amount);
                break;

            case 'h': // Help
                write(fdcnx, "---------\n", 10);
                write(fdcnx, "  --t transaction\n", 19);
                write(fdcnx, "  --h help\n", 12);
                write(fdcnx, "---------\n", 10);
                break;

            default:
                break;
        }

        // Clear reading
        _network_clean_read(fdcnx, &cmd);
    }

    printf("Bye fdcnx(%i) !\n", fdcnx);
    close(fdcnx);

    return NULL;
}

int network_constructor()
{
    struct addrinfo hints;
    struct addrinfo *result;
    int addrinfo_error;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; //Server Mode

    addrinfo_error = getaddrinfo(NULL, PORT, &hints, &result);
    if (addrinfo_error == -1)
        return INTERNAL_ERROR;

    int cnx = 0;
    struct addrinfo *rp;
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        cnx = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cnx == -1)
            continue;

        int value = 1;
        if (setsockopt(cnx, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) == -1)
            return INTERNAL_ERROR;

        if (bind(cnx, rp->ai_addr, sizeof(struct sockaddr)) == 0)
            break;

        close(cnx);
    }

    if (rp == NULL)
        return INTERNAL_ERROR;

    if (listen(cnx, 5) == -1)
        return INTERNAL_ERROR;

    int fdcnx = 0;
    while (1)
    {
        fdcnx = accept(cnx, rp->ai_addr, &rp->ai_addrlen);
        if (fdcnx == -1)
            return INTERNAL_ERROR;
        
        pthread_t thr;
        int e = pthread_create(&thr, NULL, _network_thread, &fdcnx);
        if (e != 0)
            return INTERNAL_ERROR;
        
        continue;
    }

    // End
    freeaddrinfo(result);
    close(fdcnx);
    close(cnx);

    return SUCCESS;
}
