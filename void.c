#include <stdio.h>

int main()
{
    void **vpp;
    void *test;

    test = vpp;

    int i=3;
    int ar[i];

    ar[3] = 1;

    printf("%d\n",ar[0]);

    return 0;
}
