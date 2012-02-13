#include <stdio.h>

int main()
{
    int i;

    for(i = 18; i < 90; i++) printf("%d x %d = %d\n", i / 9, i % 9 + 1, (i / 9) * (i % 9 + 1));

    return 0;
}
