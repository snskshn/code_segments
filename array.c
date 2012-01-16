#include <stdio.h>
#include <stdlib.h>

static int tester(int a)
{
    return 4;
}

void test(int num, int array2[][num])
{
    //int temp = 4;
    int array[tester(43)];
    int i;

    for(i = 0; i < num; i++) {
	array[i] = i;
	printf("%d\n", array[i]);
    }
}

int main(int argc, char **argv)
{
    if(argc == 2) {
	test(atoi(argv[1]), NULL);
    }

    return 0;
}
