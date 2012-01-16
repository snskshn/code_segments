#include <stdio.h>
#include <stdlib.h>

int global;

void test()
{
}

int main()
{
    int local;

    int *heap = malloc(sizeof(1));

    printf("global: %p\n", &global);
    printf("local: %p\n", &local);
    printf("heap: %p\n", heap);
    printf("code: %p\n", test);

    return 0;
}
