#include <stdio.h>
#include <math.h>
#include <time.h>

int main()
{
    int i;
    time_t t1, t2;

    time(&t1);
    for(i = 0; i < 10000; i++) {
	printf("result = %f\n", sin(i * M_PI / 180) * cos(i * M_PI / 180));
    }
    time(&t2);
    printf("time spent: %.2f\n", difftime(t2, t1));	// only second resolution

    return 0;
}
