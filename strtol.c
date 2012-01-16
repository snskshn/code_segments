#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *endptr;
    char str[] = "abcd";

    long l = strtol(str, &endptr, 16);
    printf("%ld\n", l);

    return 0;
}
