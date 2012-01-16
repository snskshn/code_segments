#include <stdio.h>

int main()
{
    int test = 1;
    int i;
    
    for(i = 0; i < 20; i++) {
	test++;
	test %= 13;
	printf("%d\n", test);
    }

    return 0;
}
