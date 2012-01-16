#include <stdio.h>

int main()
{
    printf("%d\n", 2 & 15);
    printf("%d\n", 2 | 15);

    unsigned a = 2;
    unsigned b = 15;
    unsigned *test1 = &a;
    unsigned *test2 = &b;

    printf("%d\n", *test1 & *test2);
    printf("%d\n", *test1 | *test2);

    return 0;
}
