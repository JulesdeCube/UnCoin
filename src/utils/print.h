#ifndef UNCOIN__UTILS__PRINT_H_
#define UNCOIN__UTILS__PRINT_H_

#define _GNU_SOURCE

#include <sys/types.h>
#include <stdio.h>

#include "builtins/array/array.h"

#include "type.h"
#include "error.h"
#include "str.h"

typedef int color_t;

typedef void (*callback_t)(args_t);

struct s_lines_info
{
    size_t max_len;
    size_t *lens;
};

typedef struct s_lines_info lines_info_t;

struct s_choice
{
    color_t color;
    string_t name;
    callback_t callback;
};

typedef struct s_choice choice_t;
typedef choice_t *choices_t;

void print_n(string_t str, size_t nb);

void print_choices(string_t title, choices_t choices, size_t nb, size_t c);

void clean_lines(size_t nb);

void print_string(string_t title, string_t str);

error_t print_lines(string_t title, Array lines);

error_t print_box(string_t title, string_t str);

void print_error(string_t error, bool_t clean);

#endif // UNCOIN__UTILS__PRINT_H_
