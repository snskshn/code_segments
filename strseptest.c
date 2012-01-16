#include <string.h>
#include <stdio.h>

int main()
{
    char *str = "abcdefg dflkj; sldkjf";

    printf("%s\n", strsep(&str, " ;"));

    return 0;
}
