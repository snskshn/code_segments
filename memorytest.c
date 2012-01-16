#include <stdio.h>

int main()
{
    char *str = "abc";
    char str2[3] = "de";

    printf("text area: %p\n", str);
    printf("stack area: %p\n", str2);

    str[0] = '#';
    str2[0] = '@';

    printf("%s %s\n", str, str2);	// seg fault

    return 0;
}

