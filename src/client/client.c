#define _GNU_SOURCE
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

// #include "builtins/array/array.h"

typedef char *string_t;
typedef int bool_t;
typedef int color_t;
typedef void *args_t;

typedef void (*callback_t)(args_t);
typedef bool_t (*add_validator_t)(string_t, char);

struct s_choice
{
    color_t color;
    string_t name;
    callback_t callback;
};

typedef struct s_choice choice_t;
typedef choice_t *choices_t;

struct s_transaction
{
    string_t to;
    float ammout;
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
typedef split_t *splits_t;

const bool_t true = 1;
const bool_t false = 0;

#define UP_KEY 65
#define DOWN_KEY 66
#define RIGHT_KEY 67
#define LEFT_KEY 68

#define J_KEY 106
#define K_KEY 107
#define L_KEY 108
#define COLUM_KEY 59
#define BACK_KEY 127

#define ENTER_KEY 10

bool_t is_letter(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

bool_t ascii_char(char c)
{
    return c >= ' ' && c <= '~';
}

size_t string_len(string_t str, bool_t allow_esc)
{
    size_t len = 0;
    for (; *str; ++str)
    {
        if (allow_esc && *str == '\033')
            while (*str && !is_letter(*str))
                str++;
        else
            len++;
    }
    return len;
}

void print_n(size_t nb, string_t str)
{
    while (nb--)
        for (string_t s = str; *s; s++)
            putchar(*s);
}

void print_choices(string_t title, choices_t choices, size_t nb, size_t c)
{
    size_t title_len = string_len(title, true);
    size_t max_len = title_len;
    size_t *lens = malloc(nb * sizeof(size_t));
    // ╭───╮
    // │foo│
    // ╰───╯
    for (size_t i = 0; i < nb; i++)
        if ((lens[i] = string_len(choices[i].name, true)) > max_len)
            max_len = lens[i];

    printf("\033[0m╭\033[1m %s \033[0m", title);
    print_n(max_len - title_len + 2, "─");
    printf("╮\n");

    for (size_t i = 0; i < nb; i++)
        printf("│ \033[%i%sm %s%*s \033[0m │\n",
               choices[i].color,
               i == c ? ";7;1" : "",
               choices[i].name,
               (int)(max_len - lens[i]), "");

    printf("╰");
    print_n(max_len + 4, "─");
    printf("╯\n");

    free(lens);
}

void clean_lines(size_t nb)
{
    for (; nb; --nb)
        printf("\033[K\033[1A\033[K");
}

size_t choice(string_t title, choice_t *choices, size_t nb, args_t args,
              bool_t clear)
{
    char c;
    size_t choice = 0;

    while (true)
    {
        print_choices(title, choices, nb, choice);
        c = getchar();

        if (c == L_KEY || c == UP_KEY)
            choice -= !!choice;
        else if (c == K_KEY || c == DOWN_KEY)
            choice += choice < nb - 1;
        else if (c == ENTER_KEY || c == COLUM_KEY)
            break;

        clean_lines(nb + 2);
    }

    if (clear)
        clean_lines(nb + 2);

    if (choices[choice].callback != NULL)
        choices[choice].callback(args);

    return choice;
}

void print_string(string_t title, string_t str)
{
    size_t str_len = string_len(str, true);
    size_t title_len = string_len(title, true);
    size_t max_len = str_len > title_len + 2 ? str_len : title_len + 2;

    printf("╭\033[1m %s \033[0m", title);
    print_n(max_len - title_len, "─");
    printf("╮\n│ %s\033[0m%*s |\n╰",
           str,
           (int)(max_len - str_len), "");
    print_n(max_len + 2, "─");
    printf("╯\n");
}

void string_input(string_t title, string_t prefix, string_t *str,
                  add_validator_t validator, bool_t clear)
{
    if (*str == NULL)
    {
        *str = calloc(1, sizeof(char));
        if (*str == NULL)
            errx(2, "can't allocate memory");
    }

    char c;
    size_t size = string_len(*str, false) + 1;
    while (true)
    {
        string_t print;
        asprintf(&print, "%s: %s", prefix, *str);
        print_string(title, print);
        free(print);

        c = getchar();
        if (ascii_char(c) && (validator == NULL || validator(*str, c)))
        {
            *str = realloc(*str, (size + 1) * sizeof(char));
            (*str)[size - 1] = c;
            (*str)[size] = '\0';
            size++;
        }
        else if (size > 1 && c == BACK_KEY)
        {
            *str = realloc(*str, (size - 1) * sizeof(char));
            (*str)[size - 2] = '\0';
            size--;
        }
        else if (c == ENTER_KEY)
            break;

        clean_lines(3);
    }

    if (clear)
        clean_lines(3);
}

void transaction_to(args_t arg)
{
    transaction_t *trans = arg;
    string_input("Transaction > To", "To", &trans->to, NULL, true);
}

bool_t is_number(char c)
{
    return c >= '0' && c <= '9';
}

bool_t str_contain(string_t str, char c)
{
    while (*str && *str != c)
        str++;
    return *str == c;
}

bool_t float_validator(string_t str, char c)
{
    return is_number(c) || (c == '.' && !str_contain(str, c));
}

void transaction_ammout(args_t arg)
{
    transaction_t *trans = arg;
    string_t str;
    asprintf(&str, "%f", trans->ammout);
    string_input("Transaction > Ammount", "ammout",
                 &str,
                 float_validator, true);
    trans->ammout = strtof(str, NULL);
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

const string_t SPIN[6] = {"✶", "✸", "✹", "✺", "✹", "✷"};

const string_t SPIN2[12] = {"_", "_", "_", "-", "`", "`", "'", "´", "-", "_", "_", "_"};
const string_t SPIN3[8] = {
    "▰▱▱▱▱▱▱",
    "▰▰▱▱▱▱▱",
    "▰▰▰▱▱▱▱",
    "▰▰▰▰▱▱▱",
    "▰▰▰▰▰▱▱",
    "▰▰▰▰▰▰▱",
    "▰▰▰▰▰▰▰",
    "▰▱▱▱▱▱▱"};
const string_t SPIN4[12] = {"🕛", "🕚", "🕙", "🕘", "🕗", "🕖", "🕕", "🕔", "🕓", "🕒", "🕑", "🕐"};

void print_task(tasks_t tasks, size_t nb)
{
    (void)tasks;
    (void)nb;
}

#if 0
void print_box(string_t title, string_t str)
{
    string_t *lines = get_lines(str);

    printf("╭\033[1m %s \033[0m", title);
    print_n(max_len - title_len, "─");
    printf("╮\n│ %s\033[0m%*s |\n╰",
           str,
           (int)(max_len - str_len), "");
    print_n(max_len + 2, "─");
    printf("╯\n");
}
#endif

void print_error(string_t error, bool_t clean)
{
    string_t str;
    asprintf(&str, "\033[31m%s", error);

    print_string("\033[31mError", str);
    getchar();

    if (clean)
        clean_lines(3);

    free(str);
}

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
    asprintf(&str, "%f %s\n", trans->ammout, trans->to);

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

void send_transaction(args_t arg)
{
    transaction_t *trans = arg;

    if (trans->ammout < .00001)
    {
        print_error("\033[31mYou send nothing", true);
        return;
    }
    if (!*trans->to)
    {
        print_error("No address provided", true);
        return;
    }

    string_t str;
    asprintf(&str, "do you wan't to send %f to %s ?", trans->ammout, trans->to);

    bool_t v = validation("Confirme", str, "Ok", "Cancel", true);
    free(str);

    if (!v)
        return;

    printf("%s Crypting\n"
           "%s Sending\n",
           "🔒", SPIN4[0]);
    //usleep(80 * 1000);

    if (msg_transaction("localhost", trans) == true)
        trans->send = true;

    clean_lines(2);

    /*for (size_t i = 0; i < 40; i++)
    {
        printf("%s Crypting\n"
               "%s Sending\n",
               "🔒", SPIN4[i % 12]);
        usleep(80 * 1000);
        clean_lines(2);
    }*/
}

void transaction(args_t arg)
{
    (void)arg;
    transaction_t trans = {malloc(1 * sizeof(char)), 0, false};
    *trans.to = '\00';

    choice_t transaction_choices[4] = {
        {0, NULL, transaction_to},
        {0, NULL, transaction_ammout},
        {34, "ok", send_transaction},
        {31, "cancel", NULL}};

    size_t c;
    do
    {
        asprintf(&transaction_choices[0].name, "to: %s", trans.to);
        asprintf(&transaction_choices[1].name, "ammount: %f", trans.ammout);
        c = choice("Transaction", transaction_choices, 4, &trans, true);
        free(transaction_choices[0].name);
        free(transaction_choices[1].name);
    } while (c != 3 && !trans.send);

    free(trans.to);
}

int main(void)
{

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
    choice_t home_choices[4] = {{34, "Transaction", transaction},
                                {33, "Sold", NULL},
                                {32, "Option", NULL},
                                {31, "Quit", NULL}};

    printf("\033[2J\033[0;0f\033[93m"
           "                                           ▄▄█▀▀▀▀▀█▄▄\n"
           " _____ _____ _____ _____     _           ▄█▀    █    ▀█▄\n"
           "|  _  |   | |  _  |     |___|_|___ ___  ▄█     █ █     █▄\n"
           "|     | | | |     |   --| . | |   |_ -| █    ▄█▄▄▄█▄    █\n"
           "|__|__|_|___|__|__|_____|___|_|_|_|___| ▀█   ▄▀   ▀▄   █▀\n"
           "                                         ▀█▄▀▀▀   ▀▀▀▄█▀\n\033[3m"
           " client V1.0\033[23m                               ▀▀█▄▄▄▄▄█▀▀\n"
           "\n");

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
    return 0;
}