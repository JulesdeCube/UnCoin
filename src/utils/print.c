#include "print.h"

void print_n(string_t str, size_t nb)
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
    print_n("─", max_len - title_len + 2);
    printf("╮\n");

    for (size_t i = 0; i < nb; i++)
        printf("│ \033[%i%sm %s%*s \033[0m │\n",
               choices[i].color,
               i == c ? ";7;1" : "",
               choices[i].name,
               (int)(max_len - lens[i]), "");

    printf("╰");
    print_n("─", max_len + 4);
    printf("╯\n");

    free(lens);
}

void clean_lines(size_t nb)
{
    for (; nb; --nb)
        printf("\033[K\033[1A\033[K");
}

void print_string(string_t title, string_t str)
{
    size_t str_len = string_len(str, true);
    size_t title_len = string_len(title, true);
    size_t max_len = str_len > title_len + 2 ? str_len : title_len + 2;

    printf("╭\033[1m %s \033[0m", title);
    print_n("─", max_len - title_len);
    printf("╮\n│ %s\033[0m%*s |\n╰",
           str,
           (int)(max_len - str_len), "");
    print_n("─", max_len + 2);
    printf("╯\n");
}

void _print_box_side(any_t line, any_t info)
{
    string_t *l = line;
    lines_info_t *inf = info;
    printf("│ %s\033[0m%*s |\n", *l, (int)(inf->max_len - *inf->lens), "");
    inf->lens++;
}

void array_max_lenght(any_t string, any_t max)
{
    string_t *str = string;
    lines_info_t *m = max;

    *m->lens = string_len(*str, true);

    if (*m->lens > m->max_len)
        m->max_len = *m->lens;

    m->lens++;
}

void p_free(any_t pointer)
{
    string_t *p = pointer;
    free(*p);
}

error_t print_lines(string_t title, Array lines)
{
    size_t title_len = string_len(title, true) + 2;
    lines_info_t infos = {
        0,
        malloc(array_get_size(lines) * sizeof(size_t))};

    size_t *lens = infos.lens;

    TRY_CATCH(array_foreach(lines, array_max_lenght, &infos),
              free(infos.lens));
    infos.lens = lens;

    if (infos.max_len < title_len)
        infos.max_len = title_len;

    printf("\033[0m╭\033[1m %s \033[0m──", title);
    print_n("─", infos.max_len - title_len);
    printf("╮\n");

    lines_info_t tmp = infos;
    TRY_CATCH(array_foreach(lines, _print_box_side, &tmp),
              free(infos.lens););

    printf("╰");
    print_n("─", infos.max_len + 2);
    printf("╯\n");

    free(infos.lens);

    return SUCCESS;
}

error_t print_box(string_t title, string_t str)
{
    Array lines;

    TRY(split_lines(str, &lines));

    TRY_CATCH(print_lines(title, lines),
              array_destructor(&lines, p_free));

    array_destructor(&lines, p_free);
    return SUCCESS;
}

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
