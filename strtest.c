#include <stdio.h>
#include <string.h>

int main()
{
    char test[3] = {0x33, 0x44};
    char *test2 = "\x33\x44";
    if(memcmp(test, test2, 2)) {
	puts("failed.");
    }
    printf("%d\n", sizeof(test));

    return 0;
}
