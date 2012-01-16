#include <stdio.h>

int main()
{
    double test = 123.456;

    printf("%x\n", *((char *)&test + 2));

    return 0;
}
