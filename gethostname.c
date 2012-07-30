#include <stdio.h>
#include <unistd.h>

int main()
{
	char hostname[256];

	gethostname(hostname, sizeof(hostname));

	puts(hostname);

	return 0;
}
