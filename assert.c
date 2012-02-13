#include <assert.h>
//#include <asm/unistd.h>
#include <stdio.h>

#define ASSERT(expr) if(!(expr)) __asm__( "int 0x3" )

int divide(int a, int b)
{
    ASSERT(b != 0);

    return a / b;
}

int main()
{
    divide(6, 3);
    divide(1, 0);

    return 0;
}
