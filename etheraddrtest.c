#include <stdio.h>
#include <netinet/ether.h>

int main()
{
    char buf[6];
    char str[] = "a:b:c:d:e:f";

    ether_aton_r(str, (struct ether_addr *)buf);

    printf("%s\n", ether_ntoa((struct ether_addr *)buf));

    return 0;
}
