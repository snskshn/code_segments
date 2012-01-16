#include <stdio.h>
#include <netinet/ether.h>

int main()
{
    printf("%s\n", ether_ntoa((struct ether_addr *)NULL));

    return 0;
}
