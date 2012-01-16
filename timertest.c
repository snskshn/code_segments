#include <stdio.h>

int main()
{
    static struct timer hi;

    start_timer(&hi);

    elapsed_time(&hi);


    end_timer(&hi);
