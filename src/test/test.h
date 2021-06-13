#ifndef UNCOIN__TEST__TEST_H_
#define UNCOIN__TEST__TEST_H_

#include <stdio.h>
#include <string.h>
#include <err.h>

#include "utils/type.h"
#include "builtins/buffer/buffer.h"

/**
** a test
*/
typedef void (*TestFunc)(void);

/**
** a test and it's name
*/
struct s_test
{
    char *name;
    TestFunc func;
};
typedef struct s_test Test;

/**
 * run a list of test (level is purly estetique)
 */
void run_tests(Test *tests, size_t size, unsigned char level);

// assert
void assert_equal_ul(char *name, size_t exepted, size_t got);
void assert_equal_i(char *name, int exepted, int got);
void assert_equal_b(char *name, bool_t exepted, bool_t got);
void assert_equal_p(char *name, void *exepted, void *got);
void assert_equal_s(char *name, char *exepted, char *got);

void assert_not_equal_ul(char *name, size_t exepted, size_t got);
void assert_not_equal_p(char *name, void *exepted, void *got);

void assert_equal_arr(char *name, unsigned char *exepted_arr, unsigned char *got_arr, size_t size);
void assert_equal_buffer(char *name, Buffer exepted_buf, Buffer got_buf);

void assert_not_equal_arr(char *name, unsigned char *exepted_arr, unsigned char *got_arr, size_t size);
void assert_not_equal_buffer(char *name, Buffer exepted_buf, Buffer got_buf);

#endif // UNCOIN__TEST__TEST_H_
