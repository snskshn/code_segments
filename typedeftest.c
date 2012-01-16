#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int main()
{
    typedef int (*func)(int, int);
    typedef void *(*THREAD)(void *);

    func ptr = add;

    printf("%d\n", ptr(2, 3));

    return 0;
}
