#include "test/test.h"
#include "test_constructor.h"
#include "test_conversion.h"
#include "test_operations.h"

Test tests[] = {
    {"CONSTRUCTOR", test_constructor},
    {"CONVERSION", test_conversion},
    {"OPERATIONS", test_operations}};

bool is_correct_hex(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

enum e_opt
{
    NONE = ' ',
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/'
};

typedef enum e_opt Opt;

error_t apply_opt(Opt opt, BigInt *bigint, BigInt *result)
{

    BigInt tmp;

    switch (opt)
    {
    case NONE:
        bigint_destructor(result);
        *result = *bigint;
        TRY(bigint_constructor_null(bigint));
        return SUCCESS;

    case ADD:
        tmp = *result;
        TRY(bigint_addition(*result, *bigint, result));
        bigint_destructor(&tmp);
        break;

    case SUB:
        tmp = *result;
        TRY(bigint_substraction(*result, *bigint, result));
        bigint_destructor(&tmp);
        break;

    case MUL:
        tmp = *result;
        TRY(bigint_multiplication(*result, *bigint, result));
        bigint_destructor(&tmp);
        break;

    default:
        err(1, "unkown opt");
    }

    return SUCCESS;
}

int main(void)
{
    run_tests(tests, sizeof(tests) / sizeof(Test), 1);

    BigInt bigint, result;
    TRY(bigint_constructor_null(&bigint));
    TRY(bigint_constructor_null(&result));

    char c = '0';
    Opt opt = NONE;

    do
    {
        c = getchar();

        if (c == '\n' || c == ' ')
            continue;

        if (is_correct_hex(c))
        {
            if (bigint_is_null(bigint))
            {
                bigint_destructor(&bigint);
                TRY(bigint_constructor_from_int(&bigint, hex_to_int[(int)c]));
            }
            else
            {
                BigInt tmp = bigint;
                bigint_left_shift(bigint, 4, &bigint);
                bigint_destructor(&tmp);
                u_char *data = buffer_get_data(bigint->buffer);
                *data |= hex_to_int[(int)c];
            }
        }
        else if (c == '+')
        {
            apply_opt(opt, &bigint, &result);
            opt = ADD;
        }
        else if (c == '-')
        {
            apply_opt(opt, &bigint, &result);
            opt = SUB;
        }
        else if (c == '*')
        {
            apply_opt(opt, &bigint, &result);
            opt = MUL;
        }

    } while (c != '=');

    TRY(apply_opt(opt, &bigint, &result));

    char *str;
    TRY(bigint_to_string(result, &str, NULL));
    printf("= %s\n", str);
    free(str);
    bigint_destructor(&bigint);
    bigint_destructor(&result);

    return 0;
}
