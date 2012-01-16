#include <stdio.h>

enum hi
{
    HI1,
    HI2=3
} myhi;

enum hi2
{
    HI4,
    HI3,
};

int main()
{
    enum hi test1;
    //hi test1;
    typeof(enum hi2) test2;

    test2 = HI1;


#if 0
    myhi = 2;
    int test[HI2];

    printf("%d\n", myhi);
#endif

    return 0;
}
