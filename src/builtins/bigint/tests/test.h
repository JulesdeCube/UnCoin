#ifndef UNCOIN__BUILTINS_BIGINT_TESTS__TEST_H_
#define UNCOIN__BUILTINS_BIGINT_TESTS__TEST_H_

#define _GNU_SOURCE

#include "test/test.h"
#include "../bigint.h"

void assert_equal_bigint(char *name, BigInt exepted, BigInt got);

#endif // UNCOIN__BUILTINS_BIGINT_TESTS__TEST_H_
