#include <stdlib.h>
#include <stdio.h>

int main()
{
    long int test = -19;
    long long int test2 = -19;

    printf("%ld\n", labs(test));
    printf("%lld\n", llabs(test2));

    return 0;
}

