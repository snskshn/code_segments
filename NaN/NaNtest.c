#include <stdio.h>
#include <math.h>

int main()
{
    double x;

    x = sqrt(-1);

    if((x == x) == 0)
	printf("success!!\n");

    return 0;
}
