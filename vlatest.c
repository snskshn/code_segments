#include <stdio.h>

int printstr(int length)
{
    char str[length + 1];
    int i;

    for(i = 0; i < length; i++) {
	str[i] = '1' + i;
    }

    str[length] = 0;

    printf("%s\n", str);

    return 0;
}

int main()
{
    printstr(9);

    return 0;
}
