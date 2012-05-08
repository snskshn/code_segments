#include <net/if.h>
#include <stdio.h>

int main()
{
	printf("%d\n", if_nametoindex("eth0"));

	return 0;
}
