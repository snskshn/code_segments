#include <stdio.h>

int main()
{
    int test[40];

    printf("%d\n", sizeof(&test));
    printf("%d\n", sizeof(~0));

    return 0;
}
