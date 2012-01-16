#include <stdio.h>

int main()
{
    int test = -1;

    printf("%d\n", test >> 1);
    test = 1;
    printf("%d\n", test >> 1);
    test = 2;
    printf("%d\n", test >> 1);

    printf("%d\n", (3 << 1) + 3);


    return 0;
}
