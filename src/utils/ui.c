#include "ui.h"

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
