#include <stdio.h>

enum test1{
    ABC,
    BCD,
};

enum test2 {
    DF,
    lsdk,
};

int main()
{
    enum test1;
    enum test2;

    if(typeof(test1) == typeof(test2)) {
	printf("fail\n");
    }

#if 0
    int test = 3;

    typeof(test) test2 = test;

    printf("%d\n", sizeof(typeof(test)));
    printf("%d\n", test2);
#endif

    return 0;
}
