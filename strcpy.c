#include <stdio.h>
#include <string.h>

int main()
{
    const char *const_str = "abc";
    char normal_str[] = "efg";

    strcpy(normal_str, const_str);
    strcpy(normal_str, normal_str);
    strcpy(const_str, const_str);

    return 0;
}
