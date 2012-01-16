#include <stdio.h>

struct tag_st1
{
    char c;
    double d;
};
struct tag_st1 st1={'A', 1.234};

int main()
{
    printf("addr=%p, &c=%p, &d=%p, size=%d\n", &st1, &st1.c, &st1.d, sizeof(st1));

    return 0;
}
