#include <stdio.h>

int main()
{
    FILE *fp = freopen("./test.txt", "w", stdout);
    int i;


    for(i = 0; i < 1000; i++) {
	sleep(1);
	printf("%d\n", i);
	fflush(stdout);
    }

    return 0;
}
