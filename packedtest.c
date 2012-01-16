#include <stdio.h>

struct test
{
    int a;
    char b;
    short c;
};

int main()
{
    printf("%d\n", sizeof(struct test));

    return 0;
}
