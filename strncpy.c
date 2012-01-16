#include <stdio.h>
#include <string.h>

int main()
{
    char str1[10] = "abcdefghi";
    char str2[10] = "123456789";
    strncpy(str2, str1, 3);
    puts(str2);

    return 0;
}
