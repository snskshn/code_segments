#include <stdio.h>

typedef int (*FUNC)(int, int);
typedef int (*FUNC2)(int);
typedef char (*FUNC3)(int, int, int);

int test(int a, int b)	// text segment
{
    printf("%d\n", a + b);

    return 0;
}

void func_test(int(* some)(int, int));

int main()
{
    int *ptr = test;

    static int tester = 3;	// data segment
    int *ptr2 = &tester;

    printf("in text seg: %p\n", test);
    printf("in data seg: %p\n", &tester);

    func_test(test);
    func_test((FUNC3)test);
    func_test(ptr);
    func_test(ptr2);
    //func_test((int (*)(int,int))test);

    return 0;
}
