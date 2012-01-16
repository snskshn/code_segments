#include <stdio.h>

int main()
{
    struct test{
	struct _test2{
	    int a;
	    int b;
	} test2;
    } a;

    struct bittest {
	int a:1,
	    b:1,
	    c:1,
	    d:33;
    };

    a.test2.a = 3;
    
    return 0;
}
