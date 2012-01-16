#include <stdio.h>

#define TO_STRING(a) @a
#define _(x) #x
#define test(type, num) int i; for(i = 0; i < num; i++) printf(#type"\n")

int main()
{
    //char str[] = "abc";
    //int TO_STRING(str) = 3;
    printf("%s\n", _(abc));

    return 0;
}

