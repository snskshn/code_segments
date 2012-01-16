#include <time.h>
#include <stdio.h>

int main()
{
    time_t t;
    struct tm *pt;

    time(&t);
    pt = localtime(&t);
    printf("%d.%d.%d %d:%d:%d\n", pt->tm_year + 1900, pt->tm_mon+1, pt->tm_mday,
	    pt->tm_hour, pt->tm_min, pt->tm_sec);

    return 0;
}
