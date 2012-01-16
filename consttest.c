#include <stdio.h>

int main()
{
#if 0
    const int *ptr1;
    int test = 3;

    ptr1 = &test;
    *ptr1 = 3;	// error: target is const
#endif

    const int i = 2;
    int *pi = (int *)&i;
    *pi = 3;
    printf("i=%d\n", i);
    printf("*pi=%d\n", *pi);

    const double d = 1.0;
    double *pd = (double *)&d;
    *pd = 2.3;
    printf("d=%lf\n", d);
    printf("*pd=%lf\n", *pd);

    return 0;
}
