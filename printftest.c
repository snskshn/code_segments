#include <stdio.h>

int main()
{
    char *str = "abcdefghijklmnop";

    printf("%-.*s\n",6, str);
    printf("%-.6s\n", str);

    return 0;
}
