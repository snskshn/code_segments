#include <stdio.h>

void test(int size, int array[size])
//void test(int array[size], int size)
{
    int i;

    for(i = 0; i < size; i++) {
	printf("array[%d] = %d\n", i, array[i]);
    }
}

int main()
{
    int array[20] = {2, 2, 2, 3, 3, 3, 3, 4, 4, 4};

    test(19, array);
    //test(array, 10);

    return 0;
}
