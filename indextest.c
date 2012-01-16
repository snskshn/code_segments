#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int test[atoi(argv[1])];

    printf("%d\n", test[10]);

    return 0;
}
