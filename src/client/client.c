#define _GNU_SOURCE
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>

#include "builtins/buffer/buffer.h"
#include "builtins/array/array.h"

#include "utils/type.h"
#include "utils/ui.h"
#include "utils/print.h"

struct s_transaction
{
    string_t from;
    string_t to;
    unsigned long long int ammout;
    bool_t send;
};

typedef struct s_transaction transaction_t;

struct s_task
{
    string_t icon;
    string_t name;
    size_t progress;
};

typedef struct s_task task_t;
typedef task_t *tasks_t;

struct s_split
{
    string_t start;
    size_t size;
};

typedef struct s_split split_t;

void transaction_to(args_t arg)
{
    transaction_t *trans = arg;
    string_input("Transaction > To", "To", &trans->to, NULL, true);
}

void transaction_from(args_t arg)
{
    transaction_t *trans = arg;
    string_input("Transaction > From", "From", &trans->from, NULL, true);
}

void transaction_ammout(args_t arg)
{
    transaction_t *trans = arg;
    string_t str;
    asprintf(&str, "%lld", trans->ammout);
    string_input("Transaction > Ammount", "ammout",
                 &str,
                 unsigned_validator, true);
    trans->ammout = strtod(str, NULL);
    free(str);
}

bool_t validation(string_t title, string_t msg, string_t ok, string_t cancel, bool_t clean)
{
    (void)title;
    (void)msg;
    (void)ok;
    (void)cancel;
    (void)clean;
    return true;
}

const string_t SPIN4[12] = {"🕛", "🕚", "🕙", "🕘", "🕗", "🕖", "🕕", "🕔", "🕓", "🕒", "🕑", "🕐"};

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

bool_t msg_transaction(const char *host, transaction_t *trans)
{
    char *port = "2048";

    struct addrinfo hints;
    struct addrinfo *result;
    int addrinfo_error;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo_error = getaddrinfo(host, port, &hints, &result);
    if (addrinfo_error != 0)
    {
        errx(EXIT_FAILURE, "Fail getting address for %s on port %s: %s",
             host, port, gai_strerror(addrinfo_error));
    }

    int cnx = 0;
    struct addrinfo *rp;
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        cnx = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cnx == -1)
            continue;
        if (connect(cnx, rp->ai_addr, rp->ai_addrlen) != -1)
            break;
        close(cnx);
    }

    freeaddrinfo(result);

    if (rp == NULL)
        return false;

    char *str = NULL;
    char *str1 = NULL;
    size_t len1 = 0;

    // Structure message transaction
    asprintf(&str, "%s -> %s : %lld\n", trans->from, trans->to, trans->ammout);

    // Send transaction to server
    send(cnx, str, strlen(str), 0);

    //if (error > 0)
    //    printf("Transaction sent to server\n");
    //else
    //{
    //    printf("Transaction fail to server\n");
    //    return false;
    //}

    // Read response from server
    rerecv(cnx, &str1, &len1);
    //if (error > 0)
    //    printf("%s\n", str1);
    //else
    //{
    //    printf("No message from server\n");
    //    return false;
    //}

    free(str);
    free(str1);
    close(cnx);

    return true;
}

void transaction_send(args_t arg)
{
    transaction_t *trans = arg;

    if (trans->ammout < .00001)
    {
        print_error("You send nothing", true);
        return;
    }
    if (!*trans->to)
    {
        print_error("No destination address provided", true);
        return;
    }

    if (!*trans->from)
    {
        print_error("No comming address provided", true);
        return;
    }

    string_t str;
    asprintf(&str, "do you wan't to send %llu to %s ?", trans->ammout, trans->to);

    bool_t validate = validation("Confirme", str, "Ok", "Cancel", true);
    free(str);

    if (!validate)
        return;

    printf("%s Crypting\n"
           "%s Sending\n",
           "🔒", SPIN4[0]);
    //usleep(80 * 1000);

    clean_lines(2);

    for (size_t i = 0; i < 40; i++)
    {
        printf("%s Crypting\n"
               "%s Sending\n",
               "🔒", SPIN4[i % 12]);
        usleep(80 * 1000);
        clean_lines(2);
    }

    if (msg_transaction("localhost", trans) == true)
        trans->send = true;
}

void transaction(args_t arg)
{
    (void)arg;
    transaction_t trans = {
        calloc(1, sizeof(char)),
        calloc(1, sizeof(char)),
        0,
        false};
    *trans.to = '\00';

    choice_t transaction_choices[5] = {
        {0, NULL, transaction_from},
        {0, NULL, transaction_to},
        {0, NULL, transaction_ammout},
        {34, "ok", transaction_send},
        {31, "cancel", NULL}};

    size_t c;
    do
    {
        asprintf(&transaction_choices[0].name, "from: %s", trans.from);
        asprintf(&transaction_choices[1].name, "to: %s", trans.to);
        asprintf(&transaction_choices[2].name, "ammount: %llu", trans.ammout);

        c = choice("Transaction", transaction_choices, 5, &trans, true);

        for (size_t i = 0; i < 3; i++)
            free(transaction_choices[i].name);
    } while (c != 4 && !trans.send);

    free(trans.to);
    free(trans.from);
}

file_t safe_open(string_t path)
{
    file_t file = open(path, O_CREAT);

    if (file == -1)
        err(3, "can't open \"%s\"", path);

    return file;
}

int main(void)
{
    file_t account_f = safe_open("config/accounts/account1.key");

    Buffer account_buffer;
    TRY(buffer_constructor_file(&account_buffer, account_f));
    close(account_f);

    string_t str;
    TRY_CATCH(buffer_to_hex(account_buffer, &str, NULL),
              buffer_destructor_safe(&account_buffer));

    struct termios info;
    /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    tcgetattr(STDIN_FILENO, &info);

    // backup the flags
    tcflag_t lflag = info.c_lflag;
    cc_t cc_vmin = info.c_cc[VMIN];
    cc_t cc_vtime = info.c_cc[VTIME];

    // set new flag
    info.c_lflag &= ~ICANON & ~ECHO; /* disable canonical mode  and echo mode */
    info.c_cc[VMIN] = 1;             /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;            /* no timeout */
    tcsetattr(STDIN_FILENO, TCSANOW, &info);

    printf("\033[2J\033[0;0f\033[93m"
           "                                           ▄▄█▀▀▀▀▀█▄▄\n"
           " _____ _____ _____ _____     _           ▄█▀    █    ▀█▄\n"
           "|  _  |   | |  _  |     |___|_|___ ___  ▄█     █ █     █▄\n"
           "|     | | | |     |   --| . | |   |_ -| █    ▄█▄▄▄█▄    █\n"
           "|__|__|_|___|__|__|_____|___|_|_|_|___| ▀█   ▄▀   ▀▄   █▀\n"
           "                                         ▀█▄▀▀▀   ▀▀▀▄█▀\n\033[3m"
           " client V1.0\033[23m                               ▀▀█▄▄▄▄▄█▀▀\n"
           "\n");

    choice_t home_choices[4] = {{34, "Transaction", transaction},
                                {33, "Sold", NULL},
                                {32, "Option", NULL},
                                {31, "Quit", NULL}};
    size_t c;
    do
        c = choice("Home", home_choices, 4, NULL, true);
    while (c != 3);

    printf("\nGoodbye 👋\n");

    // reset flags
    info.c_lflag = lflag;
    info.c_cc[VMIN] = cc_vmin;
    info.c_cc[VTIME] = cc_vtime;
    tcsetattr(STDIN_FILENO, TCSANOW, &info);

    free(str);
    buffer_destructor_safe(&account_buffer);
    return 0;
}
