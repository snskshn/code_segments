#include <stdio.h>

int test()
{
    static int test[3] = {1,2,3};

    return test[0]++;
}

static int print();	// must be here

int main()
{
    printf("%d\n", test());
    printf("%d\n", test());
    printf("%d\n", test());
    printf("%d\n", test());
    printf("%d\n", test());
    print();
    return 0;
}
    
static int print()
{
    printf("can be called\n");

    return 0;
}
