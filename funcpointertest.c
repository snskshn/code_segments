#include <stdio.h>

int plus(int a, int b)
{
    return a + b;
}

int minus(int a, int b)
{
    return a - b;
}

int multiply(int a, int b)
{
    return a * b;
}

typedef int (*CAL)(int, int);
typedef char * PTR;

int main()
{
    CAL fp[3] = {&plus, &minus, &multiply};
    PTR ptr[3] = {"abc", "bcd", "lskdjf"};

    int (*test)(int, int);
    test = &minus;

    printf("%p\n", test);	// 0x011
    printf("%p\n", *******test);	// 0x011
    printf("%d\n", test(10, 20));

    char *good = "abcdefg";
    char good2[] = "abcdefg";

    printf("%d\n", sizeof(good2));
    printf("%d\n", sizeof(&good2[0]));
    printf("%d\n", sizeof(good));
    printf("%d\n", sizeof(&good[0]));

    int i;
    for(i = 0; i < 3; i++) {
	//printf("ptr[%d] = %d\n", i, fp[i](30, 10));
	printf("ptr[%d] = %d\n", i, (****************fp[i])(30, 10));
	//printf("%s\n", ptr[i]);
    }

    return 0;
}


    

