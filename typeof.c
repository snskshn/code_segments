#include <stdio.h>

int main()
{
    long long a;
    typeof(a) b;
    void *ptr = &a;

    a = 3;
    b = 4;

    printf("%lld(%d), %lld(%d)\n", a, sizeof(a), b, sizeof(b));
    
    typeof(ptr) d;
    printf("%d\n", sizeof(d));

    return 0;
}
