#include "test.h"

// print a line of `count` of `car`
void print_sep(char car, size_t count)
{
    while (count--)
        putchar(car);
    putchar('\n');
}

// separator by level
char separators[] = {'#', '=', '-'};

// test decorator
void run_test(Test test, unsigned char level)
{
    // get the seperator base on the level
    char sep = separators[level];
    // calcule the max len
    int size = 80 - level * 20;
    // get string len
    int len = strlen(test.name);

    // first line separator
    print_sep(sep, size);

    // print line sepator only on first level
    if (!level)
        putchar('\n');

    // print the function name center
    printf("%*c%s\n", (size - len) / 2, ' ', test.name);

    // print line sepator only on first level
    if (!level)
        putchar('\n');

    // title 2nd line
    print_sep(sep, size);

    // call the function
    test.func();
    // add return line
    putchar('\n');
    // last separator
    if (level < 2)
        print_sep(sep, size);
    // add line end base on the level
    print_sep('\n', 2 - level);
}

void run_tests(Test *tests, size_t size, unsigned char level)
{
    // run all test of the list
    for (size_t i = 0; i < size; i++)
        run_test(tests[i], level);
}

void assert_equal_ul(string_t name, size_t exepted, size_t got)
{
    if (exepted != got)
    {
        putchar('X');
        putchar('\n');
        errx(1, "ASSERT ERROR: %s. exepted: %lu, got: %lu\n", name, exepted, got);
    }
    else
        putchar('.');
}

void assert_equal_b(string_t name, bool_t exepted, bool_t got)
{
    if (exepted != got)
    {
        putchar('X');
        putchar('\n');
        errx(1,
             "ASSERT ERROR: %s. exepted: %s, got: %s\n",
             name,
             exepted ? "true" : "false",
             got ? "true" : "false");
    }
    else
        putchar('.');
}

void assert_equal_p(string_t name, void *exepted, void *got)
{
    if (exepted != got)
    {
        putchar('X');
        putchar('\n');
        errx(1, "ASSERT ERROR: %s. exepted: %p, got: %p\n", name, exepted, got);
    }
    else
        putchar('.');
}

void assert_not_equal_ul(string_t name, size_t exepted, size_t got)
{
    if (exepted == got)
    {
        putchar('X');
        putchar('\n');
        errx(1, "ASSERT ERROR: %s. %lu is same as %lu\n", name, exepted, got);
    }
    else
        putchar('.');
}

void assert_not_equal_p(string_t name, void *exepted, void *got)
{
    if (exepted == got)
    {
        putchar('X');
        putchar('\n');
        errx(1, "ASSERT ERROR: %s. %p is same as %p\n", name, exepted, got);
    }
    else
        putchar('.');
}

void assert_equal_arr(string_t name, unsigned char *exepted_arr, unsigned char *got_arr, size_t size)
{

    for (size_t i = 0; i < size; i++)
    {
        char exepted = exepted_arr[i];
        char got = got_arr[i];
        if (exepted != got)
        {
            putchar('X');
            putchar('\n');
            errx(1, "ASSERT ERROR: %s. exepted: %x, got: %x, at %lu index.\n",
                 name,
                 exepted,
                 got,
                 i);
        }
    }

    putchar('.');
}

void assert_not_equal_arr(string_t name, unsigned char *exepted_arr, unsigned char *got_arr, size_t size)
{

    for (size_t i = 0; i < size; i++)
    {
        char exepted = exepted_arr[i];
        char got = got_arr[i];
        if (exepted != got)
        {
            putchar('.');
            return;
        }
    }
    putchar('X');
    putchar('\n');
    errx(1, "ASSERT ERROR: %s. they are the same, got first: %s, got second: %s\n",
         name,
         exepted_arr,
         got_arr);
}

void assert_equal_s(string_t name, string_t exepted, string_t got)
{
    if (got != NULL && strcmp(exepted, got))
    {
        putchar('X');
        putchar('\n');
        errx(1,
             "ASSERT ERROR: %s. exepted: %s, got: %s.\n",
             name, exepted, got);
    }

    putchar('.');
}

void assert_equal_i(string_t name, int exepted, int got)
{
    if (exepted != got)
    {
        putchar('X');
        putchar('\n');
        errx(1, "ASSERT ERROR: %s. exepted: %i, got: %i\n", name, exepted, got);
    }
    else
        putchar('.');
}
