#include <stdio.h>

struct test
{
    int a;
    int b;
    int c;
};

int main()
{
    int temp[3] = {3, 4, 6};

    struct test _test = {.a = temp[0], .b = temp[1], .c = temp[3],};

    return 0;
}
