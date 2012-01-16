#include <stdio.h>

void plusref2(int &a);

int main()
{
    int i;

    i = 0;
    plusref2(i);
    printf("result = %d\n", i);

    return 0;
}

void plusref2(int &a)
{
    a = a + 1;
}
