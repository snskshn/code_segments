#include <stdio.h>

int main()
{
    int signal = 1;
    int signal2 = 129;

    if((char)signal > (char)signal2) {
	printf("success\n");
	printf("signal: %d\n", (char)signal);
	printf("signal2: %d\n", (char)signal2);
    }

    return 0;
}
