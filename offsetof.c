#include <stdio.h>
#include <stddef.h>

#pragma pack(1)

struct test
{
    int i;
    char c;
    double f;
};

int main()
{
    printf("%d\n", offsetof(struct test, f));

    return 0;
}

