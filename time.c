#include <stdio.h>
#include <time.h>

int main()
{
    time_t t;

    time(&t);
    printf("%s", ctime(&t));	// including newline

    return 0;
}
