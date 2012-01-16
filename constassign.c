#include <stdio.h>

int main()
{
    int ar[5] = {1, 2, 3, 4, 5};

    int *pi1 = &ar[0];
    const int *pi2;

    pi2 = pi1;
    pi1 = pi2;	// warning!
    pi1 = (int *)pi2;

    return 0;
}
