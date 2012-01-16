#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    char *ip = "10.51.51.1";
    struct in_addr addr;
    // addr.s_addr;

    inet_aton(ip, &addr);

    union {
	int a;
	unsigned int b;
	char c;
    } test;

    test.a = -1;

    printf("%c\n", test.c);

    return 0;
}
