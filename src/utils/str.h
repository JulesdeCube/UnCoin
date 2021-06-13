#ifndef UNCOIN__UTILS__STR_H_
#define UNCOIN__UTILS__STR_H_

#include <sys/types.h>
#include <stdlib.h>

#include "builtins/array/array.h"

#include "type.h"
#include "error.h"

bool_t is_letter(char c);

bool_t ascii_char(char c);

bool_t is_number(char c);

size_t string_len(string_t str, bool_t allow_esc);

bool_t str_contain(string_t str, char c);

bool_t float_validator(string_t str, char c);

bool_t unsigned_validator(string_t str, char c);

error_t sub_str(string_t str, size_t size, string_t *result);

error_t split_lines(string_t str, Array *result);

#endif // UNCOIN__UTILS__STR_H_
