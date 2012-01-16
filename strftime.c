#include <stdio.h>
#include <time.h>

int main()
{
    time_t t;
    char format[128];

    time(&t);
    strftime(format, 128, "%Y %B %d %A %l:%M:%S %p", localtime(&t));
    puts(format);

    return 0;
}
