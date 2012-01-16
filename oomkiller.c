#include <stdio.h>
#include <stdlib.h>

#define MAX (unsigned)1024 * 1024 * 768 * 3

int main()
{
    char *str = calloc(1, MAX);
    if(!str) {
	perror("calloc");
    }
    int offset = 1;
    int i;

    for(i = 0; i < 31; i++) {
	offset = 1 << i;
	printf("%d:%d\n", i, str[offset]);
    }

    free(str);

    return 0;
}

