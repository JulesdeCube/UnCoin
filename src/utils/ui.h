#ifndef UNCOIN__UTILS__UI_H_
#define UNCOIN__UTILS__UI_H_

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "builtins/array/array.h"

#include "str.h"
#include "print.h"
#include "error.h"

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

typedef bool_t (*add_validator_t)(string_t, char);

size_t choice(string_t title, choice_t *choices, size_t nb, args_t args,
              bool_t clear);

void string_input(string_t title, string_t prefix, string_t *str,
                  add_validator_t validator, bool_t clear);

error_t print_lines(string_t title, Array lines);

#endif // UNCOIN__UTILS__UI_H_