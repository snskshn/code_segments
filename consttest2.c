#include <stdio.h>
#include <string.h>

void func(const int *ai)
{
    // *ai = 3;	// error!
    int *pi;
    pi = (int *)ai;
    *pi = 3;
}

int main()
{
    int i = 2;
    func(&i);
    printf("%d\n", i);

    const char *dest = "constant pointer";
    strcpy(dest, "const pointer");

    return 0;
}
