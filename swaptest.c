#include <stdio.h>

void swap(int *a, int *b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void swap2(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int a = 2;
    int b = 3;

    printf("%d, %d\n", a, b);

    swap2(&a, &b);

    printf("%d, %d\n", a, b);

    return 0;
}
