#include <stdio.h>

int main()
{
    int test2[3] = {1, 2, 3};
    //int (*test)[3];
    int **test;
    int *ptr = (int *)test2;
    *ptr = 5;

    test = &test2;
    test[0][0] = 4;

    int i;
    for(i = 0; i < 3; i++) {
	printf("test2[%d]=%d\n", i, test2[i]);
    }

    return 0;
}
