#include <stdio.h>

typedef int (*PT)[4];

int (*func())[4]
{
    static int a[3][4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    return a;
}

int main()
{
    int (*p)[4] = func();
    printf("p[1][2]: %d\n", p[1][2]);
    printf("*(*(p+1)+2: %d\n", *(*(p + 1) + 2));
    printf("p[1][2]: %d\n", p[1][2]);

    int sizer[10];

    printf("sizeof array: %d\n", sizeof(sizer));

    return 0;
}
