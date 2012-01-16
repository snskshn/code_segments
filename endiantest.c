#include <stdio.h>
#include <arpa/inet.h>

struct test {
    int a;
    char str[2];
    int c;
}; 

struct test test = {0x1234, "aB", 0xabcd};

int main()
{
    printf("size of struct test: %d\n", sizeof(struct test));

    unsigned char *ptr = &test;

    int i;

    printf("%x %x\n", 'a', 'B'); 

    // print result
    for(i = 0; i < 12; i++){
	printf("%x(%p) ", ptr[i], &ptr[i]);
    }
    puts("");

    // endian change
    test.a = htonl(htonl(test.a));
    test.c = htonl(test.c);
    
    // print result
    for(i = 0; i < 12; i++){
	printf("%x(%p) ", ptr[i], &ptr[i]);
    }
    puts("");

    return 0;
}
