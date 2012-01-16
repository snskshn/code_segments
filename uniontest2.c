#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    union _test
    {
	int x;
	char y[4];
    } test;

    test.y[0] = 0x12;
    test.y[1] = 0x34;
    test.y[2] = 0x56;
    test.y[3] = 0x78;

    printf("%x\n", test.x);

    htonl();

    //test.x = 0x12345678;
    
    //printf("%x\n", test.y[0]);
    //printf("%x\n", test.y[1]);
    //printf("%x\n", test.y[2]);
    //printf("%x\n", test.y[3]);

    return 0;
}
