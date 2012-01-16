#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    char src[100], dst[100];

    int i;
    for(i = 0; i < 100000000ll; i++) {
	memcpy(dst + (i % 2), src + (i % 2) , 18);
    }

    return 0;
}
